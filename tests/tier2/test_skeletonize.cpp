#include "cle.hpp"

#include "test_utils.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

// The exact expected skeletons come from the algorithm itself (cross-checked against a NumPy implementation of it).
// The comparison with scikit-image is done on properties: scikit-image's results depend on its sequential scan order
// and are not reproducible pixel by pixel by a parallel algorithm.

namespace
{

auto
count_objects(const cle::Device::Pointer & device, const cle::Array::Pointer & binary, const std::string & connectivity) -> float
{
  auto labels = cle::tier5::connected_components_labeling_func(device, binary, nullptr, connectivity);
  return cle::tier1::maximum_of_all_pixels_func(device, labels);
}

// number of objects (8/26-connected) and of background components (4/6-connected): objects, holes and cavities
auto
topology(const cle::Device::Pointer & device, const cle::Array::Pointer & binary) -> std::array<float, 2>
{
  auto background = cle::tier1::binary_not_func(device, binary, nullptr);
  return { count_objects(device, binary, "box"), count_objects(device, background, "sphere") };
}

auto
make_hollow_sphere(int size, float outer, float inner) -> std::vector<float>
{
  std::vector<float> data(size * size * size, 0);
  const float        c = (size - 1) / 2.0F;
  for (int z = 0; z < size; z++)
    for (int y = 0; y < size; y++)
      for (int x = 0; x < size; x++)
      {
        const float r = std::sqrt((x - c) * (x - c) + (y - c) * (y - c) + (z - c) * (z - c));
        data[(z * size + y) * size + x] = (r <= outer && r > inner) ? 1 : 0;
      }
  return data;
}

auto
make_torus(int width, int depth, float major, float minor) -> std::vector<float>
{
  std::vector<float> data(width * width * depth, 0);
  const float        c = (width - 1) / 2.0F;
  const float        cz = (depth - 1) / 2.0F;
  for (int z = 0; z < depth; z++)
    for (int y = 0; y < width; y++)
      for (int x = 0; x < width; x++)
      {
        const float d = std::sqrt((x - c) * (x - c) + (y - c) * (y - c)) - major;
        data[(z * width + y) * width + x] = (d * d + (z - cz) * (z - cz) <= minor * minor) ? 1 : 0;
      }
  return data;
}

// checks that the skeleton keeps the topology, is inside the input and cannot be thinned further
auto
check_properties(const cle::Device::Pointer & device, const cle::Array::Pointer & input, const cle::Array::Pointer & skeleton) -> void
{
  EXPECT_EQ(topology(device, skeleton), topology(device, input));

  auto outside = cle::tier1::binary_subtract_func(device, skeleton, input, nullptr);
  EXPECT_EQ(cle::tier1::maximum_of_all_pixels_func(device, outside), 0);

  auto again = cle::tier2::skeletonize_func(device, skeleton, nullptr);
  auto diff = cle::tier1::binary_xor_func(device, again, skeleton, nullptr);
  EXPECT_EQ(cle::tier1::maximum_of_all_pixels_func(device, diff), 0);
}

} // namespace

class TestSkeletonize : public ::testing::TestWithParam<std::string>
{
protected:
  cle::Device::Pointer device;

  void
  SetUp() override
  {
    cle::BackendManager::getInstance().setBackend(GetParam());
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestSkeletonize, cross2d)
{
  // clang-format off
  const std::array<float, 10 * 10> input = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const std::array<uint8_t, 10 * 10> valid = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  // clang-format on
  std::array<uint8_t, 10 * 10> output;

  auto gpu_input = cle::Array::create(10, 10, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::skeletonize_func(device, gpu_input, nullptr);

  EXPECT_EQ(gpu_output->dtype(), cle::dType::BOOL);
  gpu_output->readTo(output.data());
  EXPECT_EQ(output, valid);
  check_properties(device, gpu_input, gpu_output);
}

TEST_P(TestSkeletonize, ring2d)
{
  // clang-format off
  const std::array<uint8_t, 9 * 9> input = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const std::array<uint8_t, 9 * 9> valid = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0 };
  // clang-format on
  std::array<uint8_t, 9 * 9> output;

  auto gpu_input = cle::Array::create(9, 9, 1, 2, cle::dType::UINT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::skeletonize_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  EXPECT_EQ(output, valid);
  check_properties(device, gpu_input, gpu_output);
}

TEST_P(TestSkeletonize, solidBoxes3d)
{
  // a solid box and a 2x2x2 cube both shrink to a single voxel (scikit-image's Lee method erases both)
  std::vector<float> box(10 * 8 * 6, 0);
  for (int z = 1; z < 5; z++)
    for (int y = 2; y < 6; y++)
      for (int x = 1; x < 9; x++)
        box[(z * 8 + y) * 10 + x] = 1;
  std::vector<float> cube(6 * 6 * 6, 0);
  for (int z = 2; z < 4; z++)
    for (int y = 2; y < 4; y++)
      for (int x = 2; x < 4; x++)
        cube[(z * 6 + y) * 6 + x] = 1;

  auto gpu_box = cle::Array::create(10, 8, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_box->writeFrom(box.data());
  std::vector<uint8_t> output(box.size());
  cle::tier2::skeletonize_func(device, gpu_box, nullptr)->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], (i == (3 * 8 + 4) * 10 + 4) ? 1 : 0) << "index " << i;
  }

  auto gpu_cube = cle::Array::create(6, 6, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_cube->writeFrom(cube.data());
  output.resize(cube.size());
  cle::tier2::skeletonize_func(device, gpu_cube, nullptr)->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], (i == (3 * 6 + 3) * 6 + 3) ? 1 : 0) << "index " << i;
  }
}

TEST_P(TestSkeletonize, hollowSphere3d)
{
  // a closed surface keeps its cavity: the skeleton is a surface, not a curve
  const int  size = 24;
  const auto input = make_hollow_sphere(size, 10, 5);

  auto gpu_input = cle::Array::create(size, size, size, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::skeletonize_func(device, gpu_input, nullptr);

  EXPECT_EQ(topology(device, gpu_output), (std::array<float, 2>{ 1, 2 }));
  check_properties(device, gpu_input, gpu_output);
}

TEST_P(TestSkeletonize, torus3d)
{
  // the skeleton of a torus is a closed curve: one object without end points
  const int  width = 32;
  const int  depth = 12;
  const auto input = make_torus(width, depth, 10, 4);

  auto gpu_input = cle::Array::create(width, width, depth, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::skeletonize_func(device, gpu_input, nullptr);
  check_properties(device, gpu_input, gpu_output);

  std::vector<uint8_t> output(input.size());
  gpu_output->readTo(output.data());
  int voxels = 0;
  for (int z = 1; z < depth - 1; z++)
    for (int y = 1; y < width - 1; y++)
      for (int x = 1; x < width - 1; x++)
      {
        if (output[(z * width + y) * width + x] == 0)
        {
          continue;
        }
        voxels++;
        int neighbors = -1;
        for (int dz = -1; dz <= 1; dz++)
          for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
              neighbors += output[((z + dz) * width + y + dy) * width + x + dx];
        EXPECT_GE(neighbors, 2) << "end point at " << x << "," << y << "," << z;
      }
  EXPECT_GT(voxels, 40); // a 26-connected circle of radius 10 has about 56 voxels
}

TEST_P(TestSkeletonize, trivialInputs)
{
  // empty image, single voxel and a line that is already a skeleton are left unchanged
  std::vector<float>   input(12 * 12, 0);
  auto                 gpu_input = cle::Array::create(12, 12, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  std::vector<uint8_t> output(input.size());

  gpu_input->writeFrom(input.data());
  cle::tier2::skeletonize_func(device, gpu_input, nullptr)->readTo(output.data());
  EXPECT_EQ(std::count(output.begin(), output.end(), 1), 0);

  input[5 * 12 + 7] = 1;
  gpu_input->writeFrom(input.data());
  cle::tier2::skeletonize_func(device, gpu_input, nullptr)->readTo(output.data());
  EXPECT_EQ(std::count(output.begin(), output.end(), 1), 1);
  EXPECT_EQ(output[5 * 12 + 7], 1);

  for (int i = 1; i < 11; i++)
  {
    input[i * 12 + i] = 1; // diagonal line, through the single voxel's neighbour
  }
  input[5 * 12 + 7] = 0;
  gpu_input->writeFrom(input.data());
  cle::tier2::skeletonize_func(device, gpu_input, nullptr)->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]) << "index " << i;
  }
}

TEST_P(TestSkeletonize, nonBinaryInput)
{
  // any non-zero value is foreground, and an existing dst of another type is filled
  std::vector<float> input(16 * 12 * 6, 0);
  std::vector<float> binary(input.size(), 0);
  for (int z = 1; z < 5; z++)
    for (int y = 2; y < 10; y++)
      for (int x = 2; x < 14; x++)
      {
        input[(z * 12 + y) * 16 + x] = (x % 3 == 0) ? -2.5F : 7;
        binary[(z * 12 + y) * 16 + x] = 1;
      }

  auto gpu_input = cle::Array::create(16, 12, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_binary = cle::Array::create(16, 12, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(16, 12, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_binary->writeFrom(binary.data());
  cle::tier2::skeletonize_func(device, gpu_input, gpu_output);

  std::vector<float>   output(input.size());
  std::vector<uint8_t> valid(input.size());
  gpu_output->readTo(output.data());
  cle::tier2::skeletonize_func(device, gpu_binary, nullptr)->readTo(valid.data());
  EXPECT_GT(std::count(valid.begin(), valid.end(), 1), 0);
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]) << "index " << i;
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSkeletonize, ::testing::ValuesIn(getParameters()));
