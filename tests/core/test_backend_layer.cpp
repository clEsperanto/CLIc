#include "cle.hpp"

#include "test_utils.hpp"
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

// Regression tests for the backend and device layer: memory copies, kernel arguments, device queries,
// kernel caches and asynchronous execution.

class TestBackendLayer : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  void
  SetUp() override
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }

  // Name of the file extension used by the backend for its disk cache entries
  [[nodiscard]] auto
  cacheExtension() const -> std::string
  {
    if (backend == "cuda")
    {
      return "cubin";
    }
    if (backend == "metal")
    {
      return "msl";
    }
    return "bin";
  }

  // A kernel source unique to this process, so that it is never already in the in-memory cache
  [[nodiscard]] static auto
  uniqueSource(const std::string & body) -> std::string
  {
    static int counter = 0;
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return "// unique " + std::to_string(now) + "_" + std::to_string(counter++) + "\n" + body;
  }
};

// Reference rectangular copy on the host (shapes, origins and region in elements)
static auto
hostCopyRect(const std::vector<float> &    src,
             const std::array<size_t, 3> & src_shape,
             const std::array<size_t, 3> & src_origin,
             std::vector<float> &          dst,
             const std::array<size_t, 3> & dst_shape,
             const std::array<size_t, 3> & dst_origin,
             const std::array<size_t, 3> & region) -> void
{
  for (size_t z = 0; z < region[2]; ++z)
  {
    for (size_t y = 0; y < region[1]; ++y)
    {
      for (size_t x = 0; x < region[0]; ++x)
      {
        const size_t s = (src_origin[2] + z) * src_shape[1] * src_shape[0] + (src_origin[1] + y) * src_shape[0] + src_origin[0] + x;
        const size_t d = (dst_origin[2] + z) * dst_shape[1] * dst_shape[0] + (dst_origin[1] + y) * dst_shape[0] + dst_origin[0] + x;
        dst[d] = src[s];
      }
    }
  }
}

struct RegionCase
{
  std::array<size_t, 3> shape;
  std::array<size_t, 3> region;
  std::array<size_t, 3> src_origin;
  std::array<size_t, 3> dst_origin;
};

static const std::vector<RegionCase> region_cases = {
  { { 4, 4, 1 }, { 2, 2, 1 }, { 1, 1, 0 }, { 2, 1, 0 } }, // partial rows (row by row)
  { { 4, 4, 1 }, { 4, 2, 1 }, { 0, 1, 0 }, { 0, 2, 0 } }, // whole rows
  { { 5, 3, 3 }, { 5, 3, 2 }, { 0, 0, 1 }, { 0, 0, 0 } }, // whole slices
  { { 5, 3, 3 }, { 2, 2, 2 }, { 3, 1, 1 }, { 1, 0, 0 } }, // partial rows of a 3D region
  { { 6, 1, 1 }, { 3, 1, 1 }, { 2, 0, 0 }, { 1, 0, 0 } }, // 1D
};

TEST_P(TestBackendLayer, bufferImageRegionCopies)
{
  if (backend == "opencl" && !device->supportImage())
  {
    GTEST_SKIP() << "Device does not support images";
  }

  for (const auto & c : region_cases)
  {
    const size_t       size = c.shape[0] * c.shape[1] * c.shape[2];
    const auto         dim = cle::shape_to_dimension(c.shape[0], c.shape[1], c.shape[2]);
    std::vector<float> input(size);
    std::iota(input.begin(), input.end(), 1.0f);

    auto buffer = cle::Array::create(c.shape[0], c.shape[1], c.shape[2], dim, cle::dType::FLOAT, cle::mType::BUFFER, device);
    auto image = cle::Array::create(c.shape[0], c.shape[1], c.shape[2], dim, cle::dType::FLOAT, cle::mType::IMAGE, device);
    auto buffer_out = cle::Array::create(buffer);
    buffer->writeFrom(input.data());
    image->fill(0);
    buffer_out->fill(0);

    // buffer -> image with origins on both sides
    buffer->copyTo(image, c.region, c.src_origin, c.dst_origin);
    std::vector<float> expected_image(size, 0);
    hostCopyRect(input, c.shape, c.src_origin, expected_image, c.shape, c.dst_origin, c.region);
    std::vector<float> result(size, -1);
    image->readTo(result.data());
    EXPECT_EQ(result, expected_image) << "buffer -> image, shape " << c.shape[0] << "x" << c.shape[1] << "x" << c.shape[2];

    // image -> buffer, copying back the same block to the source origin
    image->copyTo(buffer_out, c.region, c.dst_origin, c.src_origin);
    std::vector<float> expected_buffer(size, 0);
    hostCopyRect(expected_image, c.shape, c.dst_origin, expected_buffer, c.shape, c.src_origin, c.region);
    buffer_out->readTo(result.data());
    EXPECT_EQ(result, expected_buffer) << "image -> buffer, shape " << c.shape[0] << "x" << c.shape[1] << "x" << c.shape[2];

    // whole-array copies (packed buffer side), directly and through copy_func
    buffer->copyTo(image);
    image->readTo(result.data());
    EXPECT_EQ(result, input) << "whole buffer -> image, shape " << c.shape[0] << "x" << c.shape[1] << "x" << c.shape[2];
    buffer_out->fill(0);
    image->copyTo(buffer_out);
    buffer_out->readTo(result.data());
    EXPECT_EQ(result, input) << "whole image -> buffer, shape " << c.shape[0] << "x" << c.shape[1] << "x" << c.shape[2];
    auto copied = cle::tier1::copy_func(device, image, nullptr);
    copied->readTo(result.data());
    EXPECT_EQ(result, input) << "copy_func image -> buffer, shape " << c.shape[0] << "x" << c.shape[1] << "x" << c.shape[2];
  }
}

TEST_P(TestBackendLayer, bufferRegionCopies)
{
  for (const auto & c : region_cases)
  {
    const size_t       size = c.shape[0] * c.shape[1] * c.shape[2];
    const auto         dim = cle::shape_to_dimension(c.shape[0], c.shape[1], c.shape[2]);
    std::vector<float> input(size);
    std::iota(input.begin(), input.end(), 1.0f);

    auto src = cle::Array::create(c.shape[0], c.shape[1], c.shape[2], dim, cle::dType::FLOAT, cle::mType::BUFFER, device);
    auto dst = cle::Array::create(src);
    src->writeFrom(input.data());
    dst->fill(0);
    src->copyTo(dst, c.region, c.src_origin, c.dst_origin);

    std::vector<float> expected(size, 0);
    hostCopyRect(input, c.shape, c.src_origin, expected, c.shape, c.dst_origin, c.region);
    std::vector<float> result(size, -1);
    dst->readTo(result.data());
    EXPECT_EQ(result, expected);

    // full copy
    auto full = cle::Array::create(src);
    src->copyTo(full);
    full->readTo(result.data());
    EXPECT_EQ(result, input);
  }
}

TEST_P(TestBackendLayer, unsignedAndSizeTKernelParameters)
{
  const std::string name = "set_value";
  const std::string native_source = uniqueSource(R"(__kernel void set_value(__global float* dst, const unsigned int value) {
    dst[get_global_id(0)] = (float) value;
  })");

  auto arr = cle::Array::create(8, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  arr->fill(0);
  std::vector<float> result(arr->size());

  // size_t has the size of a device pointer: it must still be passed as a scalar value
  cle::native_execute(device, { name, native_source }, { { "dst", arr }, { "value", size_t{ 7 } } }, { arr->size(), 1, 1 });
  arr->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 7.0f; }));

  // execute() with non-array, non-float, non-int parameters
  const std::string clij_source =
    R"(__kernel void set_value(IMAGE_dst_TYPE dst, const unsigned int value) {
    const int x = get_global_id(0);
    WRITE_IMAGE(dst, POS_dst_INSTANCE(x, 0, 0, 0), CONVERT_dst_PIXEL_TYPE(value));
  })";
  cle::execute(device, { name, clij_source }, { { "dst", arr }, { "value", 3u } }, { arr->size(), 1, 1 });
  arr->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 3.0f; }));

  cle::execute(device, { name, clij_source }, { { "dst", arr }, { "value", size_t{ 5 } } }, { arr->size(), 1, 1 });
  arr->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 5.0f; }));

  // invalid parameters are reported instead of crashing
  const size_t too_large = static_cast<size_t>(std::numeric_limits<unsigned int>::max()) + 1;
  EXPECT_THROW(cle::native_execute(device, { name, native_source }, { { "dst", arr }, { "value", too_large } }, { arr->size(), 1, 1 }),
               std::out_of_range);
  EXPECT_THROW(cle::execute(device, { name, clij_source }, { { "dst", cle::Array::Pointer{} }, { "value", 1u } }, { 8, 1, 1 }),
               std::invalid_argument);
}

TEST_P(TestBackendLayer, deviceQueries)
{
  auto & backend_ref = cle::BackendManager::getInstance().getBackend();

  // devices are created once and shared by the queries
  EXPECT_EQ(backend_ref.getDevice("", "gpu"), device);
  const auto devices_a = backend_ref.getDevices("all");
  const auto devices_b = backend_ref.getDevices("all");
  EXPECT_EQ(devices_a, devices_b);
  EXPECT_EQ(backend_ref.getDeviceFromIndex(0, "all"), devices_a.front());
  EXPECT_EQ(backend_ref.getDevice(device->getName(), "all")->getName(), device->getName());

  // device information
  const auto name = device->getName();
  auto       lower_name = name;
  std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) { return std::tolower(c); });
  EXPECT_FALSE(name.empty());
  EXPECT_EQ(device->getName(true), lower_name);

  const auto platform = device->getPlatform();
  EXPECT_FALSE(platform.empty());
  EXPECT_TRUE(std::all_of(platform.begin(), platform.end(), [](unsigned char c) { return std::isprint(c); }))
    << "Platform name is not printable: " << platform;

  EXPECT_GT(device->getMaximumWorkGroupSize(), 0u);
  EXPECT_GT(device->getMaximumBufferSize(), 0u);
  EXPECT_GT(device->getLocalMemorySize(), 0u);
  EXPECT_EQ(device->getInfoExtended().find("Max Work Group Size: 0\n"), std::string::npos);
  EXPECT_EQ(device->getNbDevicesFromContext(), 1u);

  // synchronisation with the device, and clear error on an invalid one
  EXPECT_NO_THROW(backend_ref.syncToStream(device, 0));
  EXPECT_THROW(backend_ref.syncToStream(nullptr, 0), std::invalid_argument);
}

TEST_P(TestBackendLayer, diskCacheHash)
{
  // stable across platforms and runs
  EXPECT_EQ(cle::DiskCache::hash("abc"), "e71fa2190541574b-3");
  EXPECT_EQ(cle::DiskCache::hash(""), "cbf29ce484222325-0");
  EXPECT_NE(cle::DiskCache::hash("abc"), cle::DiskCache::hash("abd"));

  // device hash is computed once and identifies the device
  const auto & hash = deviceHash(*device);
  EXPECT_FALSE(hash.empty());
  EXPECT_EQ(&hash, &deviceHash(*device));
  EXPECT_EQ(hash, cle::DiskCache::hash(device->getInfo()));
}

TEST_P(TestBackendLayer, corruptedDiskCacheEntryIsRebuilt)
{
  const bool cache_was_enabled = cle::is_cache_enabled();
  cle::use_cache(true);

  const std::string name = "add_one";
  const std::string source = uniqueSource(R"(__kernel void add_one(__global float* dst) {
    dst[get_global_id(0)] += 1.0f;
  })");

  // plant a corrupted entry where this kernel will be looked up
  auto &     disk_cache = cle::DiskCache::instance();
  const auto path = disk_cache.getFilePath(deviceHash(*device), cle::DiskCache::hash(source), cacheExtension());
  std::filesystem::create_directories(path.parent_path());
  const std::string garbage = "this is not a valid kernel binary";
  {
    std::ofstream out(path, std::ios::binary);
    out << garbage;
  }

  auto arr = cle::Array::create(16, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  arr->fill(1);
  EXPECT_NO_THROW(cle::native_execute(device, { name, source }, { { "dst", arr } }, { arr->size(), 1, 1 }));
  std::vector<float> result(arr->size());
  arr->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 2.0f; }));

  // the entry was replaced by a valid one, without leftover temporary files
  std::string content;
  {
    // closed before the end of the test: Windows cannot remove an open file
    std::ifstream in(path, std::ios::binary);
    content.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
  }
  EXPECT_FALSE(content.empty());
  EXPECT_NE(content, garbage);
  for (const auto & entry : std::filesystem::directory_iterator(path.parent_path()))
  {
    EXPECT_EQ(entry.path().string().find(".tmp"), std::string::npos) << entry.path();
  }

  // second run comes from the in-memory cache
  EXPECT_NO_THROW(cle::native_execute(device, { name, source }, { { "dst", arr } }, { arr->size(), 1, 1 }));
  arr->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 3.0f; }));

  std::filesystem::remove(path);
  cle::use_cache(cache_was_enabled);
}

TEST_P(TestBackendLayer, asynchronousExecution)
{
  // chains of kernels and transfers without waiting after each kernel
  device->setWaitToFinish(false);

  auto a = cle::Array::create(64, 33, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  a->fill(0);
  auto b = cle::Array::create(a);
  for (int i = 0; i < 25; ++i)
  {
    cle::tier1::add_image_and_scalar_func(device, a, b, 1);
    cle::tier1::add_image_and_scalar_func(device, b, a, 1);
  }
  std::vector<float> result(a->size());
  a->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 50.0f; }));

  // host write right after a kernel that reads the same buffer, then kernel and copy
  cle::tier1::add_image_and_scalar_func(device, a, b, 1);
  std::vector<float> ones(a->size(), 1.0f);
  a->writeFrom(ones.data());
  cle::tier1::add_image_and_scalar_func(device, a, b, 1);
  auto c = cle::Array::create(a);
  b->copyTo(c);
  c->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 2.0f; }));

  // fill right after a kernel writing the same buffer
  cle::tier1::add_image_and_scalar_func(device, a, b, 1);
  b->fill(9);
  b->readTo(result.data());
  EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 9.0f; }));

  device->finish();
  device->setWaitToFinish(true);
}

TEST_P(TestBackendLayer, launchShapes)
{
  // shapes that do not fit a work-group evenly, in 1D, 2D and 3D, and along y or z only
  const std::vector<std::array<size_t, 3>> shapes = { { 1000, 1, 1 }, { 1, 300, 1 }, { 1, 1, 70 }, { 37, 19, 1 }, { 17, 13, 5 } };
  for (const auto & s : shapes)
  {
    const auto dim = cle::shape_to_dimension(s[0], s[1], s[2]);
    auto       src = cle::Array::create(s[0], s[1], s[2], dim, cle::dType::FLOAT, cle::mType::BUFFER, device);
    src->fill(2);
    auto dst = cle::tier1::add_image_and_scalar_func(device, src, nullptr, 3);

    std::vector<float> result(dst->size());
    dst->readTo(result.data());
    EXPECT_TRUE(std::all_of(result.begin(), result.end(), [](float v) { return v == 5.0f; }))
      << "shape " << s[0] << "x" << s[1] << "x" << s[2];
  }
}

TEST_P(TestBackendLayer, labelAccumulationsWithPartialBlocks)
{
  // 37 x 19 does not fit a work-group / CUDA block evenly; label 2 touches the right and bottom edges, where
  // out-of-range threads would read clamped pixels and accumulate them
  const size_t          width = 37, height = 19;
  std::vector<uint32_t> labels(width * height);
  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      labels[y * width + x] = (x < 20) ? 1 : 2;
    }
  }
  auto label = cle::Array::create(width, height, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  label->writeFrom(labels.data());
  auto intensity = cle::Array::create(width, height, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  intensity->fill(1);

  // centroids: 2 labels x {x, y, z}
  auto               centroids = cle::tier4::centroids_of_labels_func(device, label, nullptr, false);
  std::vector<float> c(centroids->size());
  centroids->readTo(c.data());
  ASSERT_EQ(centroids->width(), size_t{ 2 });
  EXPECT_FLOAT_EQ(c[0], 9.5f);  // label 1, x in [0, 19]
  EXPECT_FLOAT_EQ(c[1], 28.0f); // label 2, x in [20, 36]
  EXPECT_FLOAT_EQ(c[2], 9.0f);  // y in [0, 18]
  EXPECT_FLOAT_EQ(c[3], 9.0f);

  auto stats = cle::tier3::statistics_of_labelled_pixels_func(device, intensity, label);
  EXPECT_FLOAT_EQ(stats["area"].back(), 17.0f * 19.0f);
  EXPECT_FLOAT_EQ(stats["centroid_x"].back(), 28.0f);
  EXPECT_FLOAT_EQ(stats["centroid_y"].back(), 9.0f);
  EXPECT_FLOAT_EQ(stats["bbox_max_x"].back(), 36.0f);
}

TEST_P(TestBackendLayer, globalSizeIsExact)
{
  // every backend must behave as OpenCL: exactly the requested number of work-items, and get_global_size() returning
  // the requested size (CUDA launches whole blocks, see C10)
  const std::string source = uniqueSource(R"(__kernel void count_work_items(__global int* counter, __global int* sizes) {
    atomic_add(&counter[0], 1);
    if (get_global_id(0) == 0 && get_global_id(1) == 0 && get_global_id(2) == 0) {
      sizes[0] = get_global_size(0);
      sizes[1] = get_global_size(1);
      sizes[2] = get_global_size(2);
    }
  })");
  const std::vector<std::array<size_t, 3>> ranges = { { 1000, 1, 1 }, { 37, 19, 1 }, { 37, 19, 3 }, { 1, 300, 1 }, { 1, 1, 70 } };
  for (const auto & r : ranges)
  {
    auto counter = cle::Array::create(1, 1, 1, 1, cle::dType::INT32, cle::mType::BUFFER, device);
    auto sizes = cle::Array::create(3, 1, 1, 1, cle::dType::INT32, cle::mType::BUFFER, device);
    counter->fill(0);
    sizes->fill(0);
    cle::native_execute(device, { "count_work_items", source }, { { "counter", counter }, { "sizes", sizes } }, r);

    int                count = 0;
    std::array<int, 3> reported = {};
    counter->readTo(&count);
    sizes->readTo(reported.data());
    EXPECT_EQ(count, static_cast<int>(r[0] * r[1] * r[2])) << "range " << r[0] << "x" << r[1] << "x" << r[2];
    EXPECT_EQ(reported, (std::array<int, 3>{ static_cast<int>(r[0]), static_cast<int>(r[1]), static_cast<int>(r[2]) }))
      << "range " << r[0] << "x" << r[1] << "x" << r[2];
  }
}

TEST_P(TestBackendLayer, allocationChurnAsynchronous)
{
  // arrays allocated and released inside an asynchronous chain: released memory may be reused by the next
  // allocation (stream-ordered pool on CUDA) only once the work using it is done
  device->setWaitToFinish(false);
  auto               src = cle::Array::create(256, 128, 4, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  std::vector<float> result(src->size());
  for (int i = 0; i < 40; ++i)
  {
    src->fill(static_cast<float>(i));
    auto tmp = cle::tier1::add_image_and_scalar_func(device, src, nullptr, 1); // new allocation
    auto out = cle::tier1::multiply_image_and_scalar_func(device, tmp, nullptr, 2); // new allocation
    tmp.reset(); // released while queued
    auto big = cle::Array::create(src); // same size: may reuse the block of tmp
    big->fill(-1);
    out->readTo(result.data());
    ASSERT_TRUE(std::all_of(result.begin(), result.end(), [i](float v) { return v == 2.0f * (i + 1); })) << "iteration " << i;
  }
  device->finish();
  device->setWaitToFinish(true);
}

TEST_P(TestBackendLayer, fillAllTypes)
{
  const std::vector<cle::dType> types = { cle::dType::FLOAT,  cle::dType::INT32, cle::dType::UINT32, cle::dType::INT16,
                                          cle::dType::UINT16, cle::dType::INT8,  cle::dType::UINT8 };
  for (const auto & type : types)
  {
    // negative values only for signed types (float to unsigned conversion of a negative value is undefined)
    const bool  is_unsigned = type == cle::dType::UINT32 || type == cle::dType::UINT16 || type == cle::dType::UINT8;
    const float expected = is_unsigned ? 3.0f : -3.0f;

    auto arr = cle::Array::create(7, 5, 3, 3, type, cle::mType::BUFFER, device);
    arr->fill(expected);
    auto as_float = cle::tier1::copy_func(device, arr, cle::Array::create(7, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device));
    std::vector<float> result(as_float->size());
    as_float->readTo(result.data());

    EXPECT_TRUE(std::all_of(result.begin(), result.end(), [&](float v) { return v == expected; })) << "type " << type;
  }
}

TEST_P(TestBackendLayer, launchSignatureCache)
{
  // The same kernel alternately launched with different shapes, types, memory types, parameter names and constants:
  // each launch signature must get its own program source (sizes, types and constants are baked into the defines).
  const std::string name = "set_constant";
  const std::string source = R"(__kernel void set_constant(IMAGE_dst_TYPE dst, const float offset) {
    const int x = get_global_id(0); const int y = get_global_id(1); const int z = get_global_id(2);
    WRITE_IMAGE(dst, POS_dst_INSTANCE(x, y, z, 0), CONVERT_dst_PIXEL_TYPE(VALUE + offset));
  })";
  struct Case
  {
    std::array<size_t, 3> shape;
    cle::dType            type;
    int                   value;
    float                 offset;
  };
  const std::vector<Case> cases = { { { 8, 1, 1 }, cle::dType::FLOAT, 4, 0.5f },  { { 5, 3, 1 }, cle::dType::UINT8, 7, 1 },
                                    { { 8, 1, 1 }, cle::dType::FLOAT, 9, 0.25f }, { { 3, 2, 4 }, cle::dType::INT16, -2, 0 },
                                    { { 5, 3, 1 }, cle::dType::UINT8, 7, 2 },     { { 8, 1, 1 }, cle::dType::FLOAT, 4, 0.5f } };
  for (int round = 0; round < 2; ++round)
  {
    for (const auto & c : cases)
    {
      const auto dim = cle::shape_to_dimension(c.shape[0], c.shape[1], c.shape[2]);
      auto       arr = cle::Array::create(c.shape[0], c.shape[1], c.shape[2], dim, c.type, cle::mType::BUFFER, device);
      cle::execute(device,
                   { name, source },
                   { { "dst", arr }, { "offset", c.offset } },
                   { c.shape[0], c.shape[1], c.shape[2] },
                   { 0, 0, 0 },
                   { { "VALUE", c.value } });
      auto as_float = cle::tier1::copy_func(
        device, arr, cle::Array::create(c.shape[0], c.shape[1], c.shape[2], dim, cle::dType::FLOAT, cle::mType::BUFFER, device));
      std::vector<float> result(as_float->size());
      as_float->readTo(result.data());
      const float expected = (c.type == cle::dType::FLOAT) ? c.value + c.offset : static_cast<float>(static_cast<int>(c.value + c.offset));
      EXPECT_TRUE(std::all_of(result.begin(), result.end(), [&](float v) { return v == expected; }))
        << "type " << c.type << ", value " << c.value << ", offset " << c.offset << ", got " << result.front();
    }
  }
}

TEST_P(TestBackendLayer, separableFilterPasses)
{
  // Gaussian blur through execute_separable: 1, 2 and 3 active passes, disabled passes (sigma 0 or flat
  // dimension), and in place (src == dst), compared with out of place results and a host reference
  const std::vector<std::array<size_t, 3>> shapes = { { 31, 1, 1 }, { 31, 17, 1 }, { 15, 11, 9 } };
  const std::vector<std::array<float, 3>>  sigmas = { { 2, 0, 0 }, { 0, 1.5f, 0 }, { 2, 1, 0 }, { 0, 1, 1 }, { 1, 2, 1.5f }, { 0, 0, 0 } };
  for (const auto & s : shapes)
  {
    const auto         dim = cle::shape_to_dimension(s[0], s[1], s[2]);
    std::vector<float> input(s[0] * s[1] * s[2]);
    for (size_t i = 0; i < input.size(); ++i)
    {
      input[i] = static_cast<float>((i * 37) % 101);
    }
    for (const auto & sigma : sigmas)
    {
      auto src = cle::Array::create(s[0], s[1], s[2], dim, cle::dType::FLOAT, cle::mType::BUFFER, device);
      src->writeFrom(input.data());
      auto               out = cle::tier1::gaussian_blur_func(device, src, nullptr, sigma[0], sigma[1], sigma[2]);
      std::vector<float> expected(input.size());
      out->readTo(expected.data());

      // separable passes in sequence (x then y then z), each as its own call, give the same result
      auto step = cle::Array::create(src);
      src->copyTo(step);
      for (int axis = 0; axis < 3; ++axis)
      {
        std::array<float, 3> one = { 0, 0, 0 };
        one[axis] = sigma[axis];
        step = cle::tier1::gaussian_blur_func(device, step, nullptr, one[0], one[1], one[2]);
      }
      std::vector<float> sequential(input.size());
      step->readTo(sequential.data());

      // in place
      auto inplace = cle::Array::create(src);
      src->copyTo(inplace);
      cle::tier1::gaussian_blur_func(device, inplace, inplace, sigma[0], sigma[1], sigma[2]);
      std::vector<float> result(input.size());
      inplace->readTo(result.data());

      for (size_t i = 0; i < input.size(); ++i)
      {
        ASSERT_NEAR(result[i], expected[i], 1e-4) << "in place, shape " << s[0] << "x" << s[1] << "x" << s[2] << ", index " << i;
        ASSERT_NEAR(sequential[i], expected[i], 1e-3) << "sequential, shape " << s[0] << "x" << s[1] << "x" << s[2] << ", index " << i;
      }
      if (sigma == std::array<float, 3>{ 0, 0, 0 })
      {
        EXPECT_EQ(expected, input);
      }
    }
  }
}

TEST_P(TestBackendLayer, backendCacheKey)
{
  // Backend::executeKernel with and without the optional cache_key: two sources sharing a kernel name, each with its
  // own key, alternately launched, must each run their own code
  auto &            backend_ref = cle::BackendManager::getInstance().getBackend();
  const std::string name = "write_value";
  const auto        source_for = [&](int value) {
    return uniqueSource("__kernel void write_value(__global float* dst) { dst[get_global_id(0)] = " + std::to_string(value) + ".0f; }");
  };
  const std::string source_one = source_for(1);
  const std::string source_two = source_for(2);

  auto                                     arr = cle::Array::create(16, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  const std::vector<std::shared_ptr<void>> args = { arr->get_ptr() };
  const std::vector<size_t>                sizes = { GPU_MEM_PTR_SIZE };
  std::vector<float>                       result(arr->size());
  const auto                               run_and_check = [&](const std::string & source, const std::string & key, float expected) {
    backend_ref.executeKernel(device, source, name, { arr->size(), 1, 1 }, { 0, 0, 0 }, args, sizes, key);
    arr->readTo(result.data());
    EXPECT_TRUE(std::all_of(result.begin(), result.end(), [&](float v) { return v == expected; }))
      << "key '" << key << "' expected " << expected << ", got " << result.front();
  };
  for (int round = 0; round < 2; ++round)
  {
    run_and_check(source_one, "test-key-one", 1);
    run_and_check(source_two, "test-key-two", 2);
    run_and_check(source_one, "", 1);
    run_and_check(source_two, "", 2);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBackendLayer, ::testing::ValuesIn(getParameters()));

#if USE_CUDA
TEST(TestBackendLayerCUDA, syncToConsumerStream)
{
  // DLPack-style hand-over: results produced asynchronously on the device stream, read on another (consumer) stream
  // after syncToStream. Done twice, as the event of the device is reused (C7).
  cle::BackendManager::getInstance().setBackend("cuda");
  auto & backend_ref = cle::BackendManager::getInstance().getBackend();
  auto   device = backend_ref.getDevice("", "gpu");
  auto   cuda_device = std::dynamic_pointer_cast<cle::CUDADevice>(device);
  ASSERT_NE(cuda_device, nullptr);
  ASSERT_NE(cuda_device->getCUDASyncEvent(), nullptr);
  device->setWaitToFinish(false);

  ASSERT_EQ(cuCtxSetCurrent(cuda_device->getCUDAContext()), CUDA_SUCCESS);
  CUstream consumer = nullptr;
  ASSERT_EQ(cuStreamCreate(&consumer, CU_STREAM_NON_BLOCKING), CUDA_SUCCESS);

  auto               src = cle::Array::create(1024, 1024, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  std::vector<float> host(src->size());
  for (int round = 1; round <= 2; ++round)
  {
    src->fill(0);
    for (int i = 0; i < 20 * round; ++i)
    {
      cle::tier1::add_image_and_scalar_func(device, src, src, 1); // queued, not waited for
    }
    backend_ref.syncToStream(device, reinterpret_cast<int64_t>(consumer));

    // the consumer reads on its own stream: only correct if it waited for the queued kernels
    ASSERT_EQ(cuMemcpyDtoHAsync(host.data(), reinterpret_cast<CUdeviceptr>(src->get()), host.size() * sizeof(float), consumer),
              CUDA_SUCCESS);
    ASSERT_EQ(cuStreamSynchronize(consumer), CUDA_SUCCESS);
    const float expected = 20.0f * round;
    EXPECT_TRUE(std::all_of(host.begin(), host.end(), [expected](float v) { return v == expected; })) << "round " << round;
  }

  cuStreamDestroy(consumer);
  device->finish();
  device->setWaitToFinish(true);
}

TEST(TestBackendLayerCUDA, primaryContextSharedWithOtherLibraries)
{
  // C3: CLIc works in the device's primary context, as the CUDA runtime, CuPy and PyTorch do. Another library is
  // simulated with the driver API: it retains the primary context, allocates memory and uses its own stream.
  cle::BackendManager::getInstance().setBackend("cuda");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  auto cuda_device = std::dynamic_pointer_cast<cle::CUDADevice>(device);
  ASSERT_NE(cuda_device, nullptr);
  device->setWaitToFinish(true);

  CUcontext primary = nullptr;
  ASSERT_EQ(cuDevicePrimaryCtxRetain(&primary, cuda_device->getCUDADevice()), CUDA_SUCCESS);
  EXPECT_EQ(cuda_device->getCUDAContext(), primary);

  // after a CLIc call, the context current on the thread is the one the other library expects
  auto arr = cle::Array::create(64, 32, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  arr->fill(3);
  CUcontext current = nullptr;
  ASSERT_EQ(cuCtxGetCurrent(&current), CUDA_SUCCESS);
  EXPECT_EQ(current, primary);

  // the other library's memory and stream
  ASSERT_EQ(cuCtxSetCurrent(primary), CUDA_SUCCESS);
  const size_t       count = arr->size();
  CUdeviceptr        foreign = 0;
  CUstream           foreign_stream = nullptr;
  std::vector<float> host(count, 5.0f);
  ASSERT_EQ(cuMemAlloc(&foreign, count * sizeof(float)), CUDA_SUCCESS);
  ASSERT_EQ(cuStreamCreate(&foreign_stream, CU_STREAM_NON_BLOCKING), CUDA_SUCCESS);
  ASSERT_EQ(cuMemcpyHtoD(foreign, host.data(), count * sizeof(float)), CUDA_SUCCESS);

  // import it (DLPack), process it with CLIc, hand it back
  {
    auto *  managed = new DLManagedTensorVersioned();
    int64_t shape[2] = { 32, 64 };
    int64_t strides[2] = { 64, 1 };
    managed->version = { DLPACK_MAJOR_VERSION, DLPACK_MINOR_VERSION };
    managed->dl_tensor.data = reinterpret_cast<void *>(foreign);
    managed->dl_tensor.device = { kDLCUDA, 0 };
    managed->dl_tensor.ndim = 2;
    managed->dl_tensor.dtype = cle::toDLDataType(cle::dType::FLOAT);
    managed->dl_tensor.shape = shape;
    managed->dl_tensor.strides = strides;
    managed->deleter = [](DLManagedTensorVersioned * self) { delete self; };
    auto imported = cle::Array::fromDLPack(managed, device);
    cle::tier1::add_images_weighted_func(device, imported, arr, imported, 1, 1); // 5 + 3, in place
  } // released: CLIc's queued work on it is done before the deleter runs
  ASSERT_EQ(cuMemcpyDtoH(host.data(), foreign, count * sizeof(float)), CUDA_SUCCESS);
  EXPECT_TRUE(std::all_of(host.begin(), host.end(), [](float v) { return v == 8.0f; }));

  // export a CLIc array, read it on the other library's stream, release it
  {
    device->setWaitToFinish(false);
    cle::tier1::add_image_and_scalar_func(device, arr, arr, 1); // 4, queued
    arr->syncToStream(reinterpret_cast<int64_t>(foreign_stream));
    auto * exported = arr->toDLPack();
    ASSERT_EQ(cuMemcpyDtoDAsync(foreign, reinterpret_cast<CUdeviceptr>(exported->dl_tensor.data), count * sizeof(float), foreign_stream),
              CUDA_SUCCESS);
    exported->deleter(exported); // released while the copy may still be queued: waited for before CLIc can reuse it
    device->setWaitToFinish(true);
  }
  ASSERT_EQ(cuStreamSynchronize(foreign_stream), CUDA_SUCCESS);
  ASSERT_EQ(cuMemcpyDtoH(host.data(), foreign, count * sizeof(float)), CUDA_SUCCESS);
  EXPECT_TRUE(std::all_of(host.begin(), host.end(), [](float v) { return v == 4.0f; }));

  // CLIc selecting its backend again finalizes its devices (they release the primary context): the context of the
  // other library stays usable
  const CUdevice cu_device = cuda_device->getCUDADevice();
  arr.reset();
  cuda_device.reset();
  device.reset();
  cle::BackendManager::getInstance().setBackend("cuda");
  auto device2 = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  auto again = cle::Array::create(16, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device2);
  again->fill(1);
  ASSERT_EQ(cuCtxSetCurrent(primary), CUDA_SUCCESS);
  ASSERT_EQ(cuMemcpyDtoH(host.data(), foreign, count * sizeof(float)), CUDA_SUCCESS); // foreign memory still valid
  EXPECT_EQ(host[0], 4.0f);

  cuStreamDestroy(foreign_stream);
  cuMemFree(foreign);
  cuDevicePrimaryCtxRelease(cu_device);
}
#endif
