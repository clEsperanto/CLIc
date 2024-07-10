#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestArray : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestArray, allocate)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(10, 20, 1, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

  // Allocate memory for the array
  array->allocate();

  // Check that the device pointer is not null
  EXPECT_NE(array->device(), nullptr);

  // Check that the dimensions are correct
  EXPECT_EQ(array->width(), 10);
  EXPECT_EQ(array->height(), 20);
  EXPECT_EQ(array->depth(), 1);
  EXPECT_EQ(array->dim(), 2);
  EXPECT_EQ(array->dimension(), 3);
  EXPECT_EQ(array->itemSize(), sizeof(float));
  EXPECT_EQ(array->dtype(), cle::dType::FLOAT);

  auto array_other = cle::Array::create(10, 20, 30, 3, cle::dType::UINT8, cle::mType::IMAGE, device);
  EXPECT_EQ(array_other->dtype(), cle::dType::UINT8);
  EXPECT_EQ(array_other->itemSize(), sizeof(uint8_t));
}

TEST_P(TestArray, typeDataMemory)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

  // Check that the data type is correct
  EXPECT_EQ(array->dtype(), cle::dType::FLOAT);

  // Check that the memory type is correct
  EXPECT_EQ(array->mtype(), cle::mType::IMAGE);
}

TEST_P(TestArray, allocateWrite)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Write some data to the array
  std::array<float, 10 * 20 * 30> data;
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    data[i] = static_cast<float>(i);
  }

  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::IMAGE, data.data(), device);

  // Read the data back from the array
  std::array<float, 10 * 20 * 30> read_data;
  array->read(read_data.data());

  // Check that the data was read correctly
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    EXPECT_EQ(read_data[i], static_cast<float>(i));
  }
}

TEST_P(TestArray, readWrite)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

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

TEST_P(TestArray, copyFill)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

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

TEST_P(TestArray, stringCout)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

  // Capture the output of the << operator
  testing::internal::CaptureStdout();
  std::cout << array;
  std::string output = testing::internal::GetCapturedStdout();

  // Check that the output is correct
  std::stringstream expected_output;
  expected_output << "3dArray ([10,20,30], dtype=float, mtype=Image)";
  EXPECT_EQ(output, expected_output.str());
}

TEST_P(TestArray, regionOperation)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(7, 7, 1, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

  // Write data to the array
  std::array<float, 7 * 7 * 1> input;
  std::fill(input.begin(), input.end(), static_cast<float>(-7));
  array->write(input.data());

  // Write data to a subregion of the array
  std::array<float, 5 * 2 * 1> subinput;
  std::fill(subinput.begin(), subinput.end(), static_cast<float>(-1));
  array->write(subinput.data(), { 5, 2, 1 }, { 1, 1, 0 });

  // Write a single value to the array
  float value = 19;
  array->write(&value, 5, 5, 0);

  // Read data from the array
  std::vector<float> read_array(array->size());
  array->read(read_array.data());

  // Check that the data was written and read correctly
  std::array<float, 7 * 7 * 1> valid = { -7, -7, -7, -7, -7, -7, -7, -7, -1, -1, -1, -1, -1, -7, -7, -1, -1,
                                         -1, -1, -1, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7,
                                         -7, -7, -7, -7, -7, -7, 19, -7, -7, -7, -7, -7, -7, -7, -7 };
  for (int i = 0; i < valid.size(); i++)
  {
    EXPECT_EQ(valid[i], read_array[i]);
  }

  // Read data from a subregion of the array
  std::array<size_t, 3>        region = { 2, 3, 1 };
  std::array<float, 2 * 3 * 1> subregion;
  array->read(subregion.data(), region, { 1, 1, 0 });

  // Check that the data was read correctly
  std::array<float, 7 * 7 * 1> subregion_valid = {
    -1, -1, -1, -1, -7, -7,
  };
  for (int i = 0; i < subregion.size(); i++)
  {
    EXPECT_EQ(subregion_valid[i], subregion[i]);
  }

  // Read a single value from the array
  array->read(&value, 6, 6, 0);
  EXPECT_EQ(-7, value);

  // Create a new Array
  auto copy = cle::Array::create(array);
  copy->allocate();
  copy->fill(-5);
  copy->copy(array, region, { 1, 1, 0 }, { 2, 2, 0 });

  // Read data from the copy
  std::array<float, 7 * 7 * 1> read_copy;
  array->read(read_copy.data());

  // Check that the data was copied correctly
  std::array<float, 7 * 7 * 1> copy_valid = { -7, -7, -7, -7, -7, -7, -7, -7, -1, -1, -1, -1, -1, -7, -7, -1, -5,
                                              -5, -1, -1, -7, -7, -7, -5, -5, -7, -7, -7, -7, -7, -5, -5, -7, -7,
                                              -7, -7, -7, -7, -7, -7, 19, -7, -7, -7, -7, -7, -7, -7, -7 };
  for (int i = 0; i < read_copy.size(); i++)
  {
    EXPECT_EQ(copy_valid[i], read_copy[i]);
  }
}

TEST_P(TestArray, throwErrors)
{
  GTEST_SKIP();

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);


  // Create a new Array
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);

  EXPECT_THROW(array->write(nullptr), std::runtime_error);
  EXPECT_THROW(array->write(nullptr, { 10, 10, 10 }, { 0, 0, 0 }), std::runtime_error);
  EXPECT_THROW(array->write(nullptr, 10, 5, 6), std::runtime_error);

  // Write some data to the array
  std::array<float, 10 * 20 * 30> data;
  for (int i = 0; i < 10 * 20 * 30; i++)
  {
    data[i] = static_cast<float>(i);
  }
  array->write(data.data());

  EXPECT_THROW(array->read(nullptr), std::runtime_error);
  EXPECT_THROW(array->read(nullptr, { 10, 10, 10 }, { 0, 0, 0 }), std::runtime_error);
  EXPECT_THROW(array->read(nullptr, 10, 5, 6), std::runtime_error);


  auto test_empty = cle::Array::New();
  EXPECT_THROW(test_empty->write(nullptr), std::runtime_error);
  EXPECT_THROW(test_empty->write(nullptr, { 10, 10, 10 }, { 0, 0, 0 }), std::runtime_error);
  EXPECT_THROW(test_empty->write(nullptr, 10, 5, 6), std::runtime_error);
  EXPECT_THROW(test_empty->read(nullptr), std::runtime_error);
  EXPECT_THROW(test_empty->read(nullptr, { 10, 10, 10 }, { 0, 0, 0 }), std::runtime_error);
  EXPECT_THROW(test_empty->read(nullptr, 10, 5, 6), std::runtime_error);

  auto array_other = cle::Array::create(30, 20, 10, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);
  EXPECT_THROW(array->copy(array_other), std::runtime_error);
}


std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestArray, ::testing::ValuesIn(getParameters()));
