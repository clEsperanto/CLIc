#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestCreate : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestCreate, create_dst_1)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Create a new Array
  auto input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto output = cle::Array::create(3, 5, 10, 3, cle::dType::UINT16, cle::mType::BUFFER, device);

  EXPECT_THROW(cle::tier0::create_dst(nullptr, output, 2, 3, 4, cle::dType::UINT8), std::runtime_error);
  EXPECT_NO_THROW(cle::tier0::create_dst(input, output, 2, 3, 4, cle::dType::UINT8));

  // Check that the dimensions are correct
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->shortType(), "us");
}

TEST_P(TestCreate, create_dst_2)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Create a new Array
  auto input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

  cle::Array::Pointer output = nullptr;
  cle::tier0::create_dst(input, output, 2, 3, 4, cle::dType::UINT8);

  EXPECT_NE(output->device(), input->device());

  // Check that the dimensions are correct
  EXPECT_EQ(output->width(), 2);
  EXPECT_EQ(output->height(), 3);
  EXPECT_EQ(output->depth(), 4);
  EXPECT_EQ(output->dim(), input->dim());
  EXPECT_EQ(output->dimension(), input->dimension());
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->shortType(), "uc");

  output = nullptr;
  cle::tier0::create_dst(input, output, 2, 3, 4);

  EXPECT_NE(output->device(), input->device());

  // Check that the dimensions are correct
  EXPECT_EQ(output->width(), 2);
  EXPECT_EQ(output->height(), 3);
  EXPECT_EQ(output->depth(), 4);
  EXPECT_EQ(output->dim(), input->dim());
  EXPECT_EQ(output->dimension(), input->dimension());
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->shortType(), input->shortType());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCreate, ::testing::ValuesIn(getParameters()));