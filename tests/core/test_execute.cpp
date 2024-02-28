
#include "cle.hpp"
#include "execution.hpp"

#include <gtest/gtest.h>

class TestExecution : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestExecution, parameterType)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Test assignment of cle::Array::Pointer
  auto               ap = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::ParameterType pt1 = ap;
  ASSERT_TRUE(std::holds_alternative<cle::Array::Pointer>(pt1));

  // Test assignment of float
  float              fp = 0.5f;
  cle::ParameterType pt2 = fp;
  ASSERT_TRUE(std::holds_alternative<float>(pt2));

  // Test assignment of int
  int                ip = 2;
  cle::ParameterType pt3 = ip;
  ASSERT_TRUE(std::holds_alternative<int>(pt3));
}

TEST_P(TestExecution, parameterList)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("gfx1035", "all");
  device->setWaitToFinish(true);

  auto  src = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto  dst = cle::Array::create(3, 5, 10, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  int   slice = 2;
  float scalar = 5.0f;

  const cle::ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice }, { "scalar", scalar } };

  // Test the items in cle::ParameterList

  ASSERT_EQ(params.size(), 4);
  ASSERT_TRUE(params[0].first == "src" && std::holds_alternative<cle::Array::Pointer>(params[0].second));
  ASSERT_TRUE(params[1].first == "dst" && std::holds_alternative<cle::Array::Pointer>(params[1].second));
  ASSERT_TRUE(params[2].first == "index" && std::holds_alternative<int>(params[2].second));
  ASSERT_TRUE(params[3].first == "scalar" && std::holds_alternative<float>(params[3].second));
}

TEST_P(TestExecution, rangeArray)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("gfx1035", "all");
  device->setWaitToFinish(true);

  size_t width = 5, height = 10, depth = 15;
  auto   dst = cle::Array::create(width, height, depth, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

  const cle::RangeArray range = { dst->width(), dst->height(), dst->depth() };

  // Test the elements in cle::RangeArray
  ASSERT_EQ(range.size(), 3);
  ASSERT_EQ(range[0], width);
  ASSERT_EQ(range[1], height);
  ASSERT_EQ(range[2], depth);
}

TEST_P(TestExecution, constantList)
{
  int                     bins = 255;
  cle::ConstantType       cts = bins;
  const cle::ConstantList constants = { { "bins", cts } };

  ASSERT_EQ(constants.size(), 1);
  ASSERT_EQ(constants[0].first, "bins");
  ASSERT_EQ(constants[0].second, bins);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestExecution, ::testing::ValuesIn(getParameters()));
