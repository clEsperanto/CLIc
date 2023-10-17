#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

TEST(ArrayTest, CreateAndAccess)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);

  // Check that the dimensions are correct
  EXPECT_EQ(array->width(), 10);
  EXPECT_EQ(array->height(), 20);
  EXPECT_EQ(array->depth(), 30);
}

TEST(ArrayTest, DataTypeAndMemoryType)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);

  // Check that the data type is correct
  EXPECT_EQ(array->dtype(), cle::dType::FLOAT);

  // Check that the memory type is correct
  EXPECT_EQ(array->mtype(), cle::mType::BUFFER);
}

TEST(ArrayTest, AllocationAndDeallocation)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);

  // Allocate memory for the array
  array->allocate();

  // Check that the device pointer is not null
  EXPECT_NE(array->device(), nullptr);
}

TEST(ArrayTest, DataReadWrite)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);

  // Allocate memory for the array
  array->allocate();

  // Write some data to the array
  std::array<float, 10 * 20 * 30> data;
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    data[i] = static_cast<float>(i);
  }
  array->write(data.data());

  // Read the data back from the array
  std::array<float, 10 * 20 * 30> read_data;
  array->read(read_data.data());

  // Check that the data was read correctly
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    EXPECT_EQ(read_data[i], static_cast<float>(i));
  }
}

TEST(ArrayTest, CopyAndFill)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);

  // Allocate memory for the array
  array->allocate();

  // Write some data to the array
  std::array<float, 10 * 20 * 30> data;
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    data[i] = static_cast<float>(i);
  }
  array->write(data.data());

  // Copy array to another array
  auto array2 = cle::Array::create(array);
  array->copy(array2);

  // Read the data back from the array2 and check that the data was read correctly
  std::array<float, 10 * 20 * 30> read_data2;
  array2->read(read_data2.data());
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    EXPECT_EQ(read_data2[i], static_cast<float>(i));
  }

  // Fill the array with a value
  array->fill(42.0f);

  // Read the data back from the array and check that the data was read correctly
  std::array<float, 10 * 20 * 30> read_data;
  array->read(read_data.data());
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    EXPECT_EQ(read_data[i], 42.0f);
  }
}

TEST(ArrayTest, OutputStreamOperator)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);

  // Capture the output of the << operator
  testing::internal::CaptureStdout();
  std::cout << array;
  std::string output = testing::internal::GetCapturedStdout();

  // Check that the output is correct
  std::stringstream expected_output;
  expected_output << "Array ([10,20,30], dtype=float, mtype=Buffer)";
  EXPECT_EQ(output, expected_output.str());
}
