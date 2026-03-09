#include "gtest/gtest.h"
#include "cle.hpp"

namespace cle
{

class GPUMemoryWrapTest : public ::testing::Test
{
protected:
  Device::Pointer device;

  virtual void
  SetUp()
  {
    device = BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_F(GPUMemoryWrapTest, CreateFromGPUMemoryBuffer)
{
  // Create an array with allocated GPU memory
  auto original = Array::create(32, 32, 1, 2, dType::FLOAT, mType::BUFFER, device);
  ASSERT_NE(original, nullptr);
  ASSERT_TRUE(original->ownsMemory());

  // Get the GPU memory pointer
  auto gpu_ptr = original->get_ptr();
  ASSERT_NE(gpu_ptr, nullptr);

  // Create a new array that wraps the same GPU memory
  auto wrapped = Array::createFromGPUMemory(32, 32, 1, 2, dType::FLOAT, mType::BUFFER, gpu_ptr, device);
  ASSERT_NE(wrapped, nullptr);
  ASSERT_FALSE(wrapped->ownsMemory());

  // Verify dimensions
  ASSERT_EQ(wrapped->width(), 32);
  ASSERT_EQ(wrapped->height(), 32);
  ASSERT_EQ(wrapped->depth(), 1);
  ASSERT_EQ(wrapped->dtype(), dType::FLOAT);
  ASSERT_EQ(wrapped->mtype(), mType::BUFFER);
}

TEST_F(GPUMemoryWrapTest, WrappedArrayPointsToSameMemory)
{
  // Create an array and fill it with data
  auto original = Array::create(16, 16, 1, 2, dType::FLOAT, mType::BUFFER, device);
  original->fill(3.14f);

  // Wrap the GPU memory
  auto gpu_ptr = original->get_ptr();
  auto wrapped = Array::createFromGPUMemory(16, 16, 1, 2, dType::FLOAT, mType::BUFFER, gpu_ptr, device);

  // Read from wrapped array should get the same data
  std::vector<float> original_data(256);
  std::vector<float> wrapped_data(256);
  
  original->readTo(original_data.data());
  wrapped->readTo(wrapped_data.data());

  // Both should have the value we filled with
  for (size_t i = 0; i < 256; ++i)
  {
    EXPECT_FLOAT_EQ(original_data[i], 3.14f);
    EXPECT_FLOAT_EQ(wrapped_data[i], 3.14f);
  }
}

TEST_F(GPUMemoryWrapTest, CreateFromGPUMemoryNullPointerThrows)
{
  // Creating with null GPU memory pointer should throw
  EXPECT_THROW(Array::createFromGPUMemory(32, 32, 1, 2, dType::FLOAT, mType::BUFFER, nullptr, device),
               std::runtime_error);
}

TEST_F(GPUMemoryWrapTest, CreateFromGPUMemoryNullDeviceThrows)
{
  // Creating with null device pointer should throw
  auto allocated = Array::create(32, 32, 1, 2, dType::FLOAT, mType::BUFFER, device);
  auto gpu_ptr = allocated->get_ptr();
  
  EXPECT_THROW(Array::createFromGPUMemory(32, 32, 1, 2, dType::FLOAT, mType::BUFFER, gpu_ptr, nullptr),
               std::runtime_error);
}

TEST_F(GPUMemoryWrapTest, MemoryOwnershipTracking)
{
  // Array created normally should own memory
  auto owned = Array::create(16, 16, 1, 2, dType::FLOAT, mType::BUFFER, device);
  EXPECT_TRUE(owned->ownsMemory());

  // Array wrapped from external memory should not own it
  auto gpu_ptr = owned->get_ptr();
  auto wrapped = Array::createFromGPUMemory(16, 16, 1, 2, dType::FLOAT, mType::BUFFER, gpu_ptr, device);
  EXPECT_FALSE(wrapped->ownsMemory());
}

TEST_F(GPUMemoryWrapTest, CreateFromGPUMemoryDifferentDataTypes)
{
  // Test with different data types
  std::vector<dType> types = {dType::INT8, dType::UINT8, dType::INT16, dType::UINT16, dType::INT32, dType::UINT32, dType::FLOAT};

  for (auto dtype : types)
  {
    auto original = Array::create(8, 8, 1, 2, dtype, mType::BUFFER, device);
    auto gpu_ptr = original->get_ptr();
    auto wrapped = Array::createFromGPUMemory(8, 8, 1, 2, dtype, mType::BUFFER, gpu_ptr, device);

    EXPECT_EQ(wrapped->dtype(), dtype);
    EXPECT_FALSE(wrapped->ownsMemory());
  }
}

} // namespace cle
