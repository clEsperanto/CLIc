#include "cle.hpp"

#include "test_utils.hpp"

#include <array>
#include <gtest/gtest.h>
#include <numeric>

class TestDLPack : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

// ─── dtype mapping ────────────────────────────────────────────────────────────

TEST_P(TestDLPack, RoundTripFloat)
{
  auto dl = cle::toDLDataType(cle::dType::FLOAT);
  EXPECT_EQ(dl.code, kDLFloat);
  EXPECT_EQ(dl.bits, 32);
  EXPECT_EQ(cle::fromDLDataType(dl), cle::dType::FLOAT);
}

TEST_P(TestDLPack, RoundTripInt8)
{
  auto dl = cle::toDLDataType(cle::dType::INT8);
  EXPECT_EQ(dl.code, kDLInt);
  EXPECT_EQ(dl.bits, 8);
  EXPECT_EQ(cle::fromDLDataType(dl), cle::dType::INT8);
}

TEST_P(TestDLPack, RoundTripUInt16)
{
  auto dl = cle::toDLDataType(cle::dType::UINT16);
  EXPECT_EQ(dl.code, kDLUInt);
  EXPECT_EQ(dl.bits, 16);
  EXPECT_EQ(cle::fromDLDataType(dl), cle::dType::UINT16);
}

// ─── toDLPack ─────────────────────────────────────────────────────────────────

TEST_P(TestDLPack, ImportMetadataCorrect1D)
{
  auto arr = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  auto * managed = arr->toDLPack();
  ASSERT_NE(managed, nullptr);

  const auto & t = managed->dl_tensor;
  EXPECT_EQ(t.ndim, 1);
  EXPECT_EQ(t.shape[0], 10);
  EXPECT_EQ(t.dtype.code, kDLFloat);
  EXPECT_EQ(t.dtype.bits, 32);
  EXPECT_NE(t.data, nullptr);

  managed->deleter(managed); // must not crash / leak
}

TEST_P(TestDLPack, ImportMetadataCorrect3D)
{
  auto arr = cle::Array::create(4, 3, 2, 3, cle::dType::UINT8, cle::mType::BUFFER, device);

  auto * managed = arr->toDLPack();
  ASSERT_NE(managed, nullptr);

  const auto & t = managed->dl_tensor;
  EXPECT_EQ(t.ndim, 3);
  EXPECT_EQ(t.shape[0], 2); // or z,y,x depending on your convention
  EXPECT_EQ(t.shape[1], 3);
  EXPECT_EQ(t.shape[2], 4);

  managed->deleter(managed);
}

TEST_P(TestDLPack, ImportStridesAreContiguous)
{
  auto arr = cle::Array::create(5, 4, 1, 2, cle::dType::INT32, cle::mType::BUFFER, device);

  auto * managed = arr->toDLPack();
  ASSERT_NE(managed, nullptr);

  const auto & t = managed->dl_tensor;
  ASSERT_NE(t.strides, nullptr);
  // row-major contiguous: strides[last] == 1, strides[i] == strides[i+1] * shape[i+1]
  EXPECT_EQ(t.strides[1], 1);
  EXPECT_EQ(t.strides[0], t.shape[1]);

  managed->deleter(managed);
}

TEST_P(TestDLPack, ImportImageTypeThrows)
{
  auto arr = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::IMAGE, device);
  EXPECT_THROW((void)arr->toDLPack(), std::runtime_error);
}

// ─── fromDLPack ───────────────────────────────────────────────────────────────

TEST_P(TestDLPack, ExportRoundTripData)
{

  // Create source array with known data
  std::vector<float> src(12);
  std::iota(src.begin(), src.end(), 0.f); // 0,1,2,...,11
  auto original = cle::Array::create(12, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, src.data(), device);

  // Export → import
  auto * managed = original->toDLPack();
  auto   imported = cle::Array::fromDLPack(managed, device);

  // Read back and compare
  std::vector<float> dst(12);
  imported->readTo(dst.data());

  for (size_t i = 0; i < src.size(); ++i)
    EXPECT_FLOAT_EQ(dst[i], src[i]) << "mismatch at index " << i;
}

TEST_P(TestDLPack, ExportShapePreserved)
{
  auto arr = cle::Array::create(6, 4, 2, 3, cle::dType::UINT16, cle::mType::BUFFER, device);

  auto * managed = arr->toDLPack();
  auto   imported = cle::Array::fromDLPack(managed, device);

  EXPECT_EQ(imported->width(), arr->width());
  EXPECT_EQ(imported->height(), arr->height());
  EXPECT_EQ(imported->depth(), arr->depth());
  EXPECT_EQ(imported->dtype(), arr->dtype());
}

TEST_P(TestDLPack, ExportNullThrows)
{
  EXPECT_THROW(cle::Array::fromDLPack(nullptr, device), std::invalid_argument);
}


INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDLPack, ::testing::ValuesIn(getParameters()));
