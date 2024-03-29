#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestBoundingBox : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> input2d = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<float, 5 * 5 * 2> input3d = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestBoundingBox, execute2d)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input2d.data());

  auto output = cle::tier3::bounding_box_func(device, gpu_input);

  std::vector<float> valid = { 1, 1, 0, 2, 2, 0 };
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestBoundingBox, execute3d)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 2, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input3d.data());

  auto output = cle::tier3::bounding_box_func(device, gpu_input);

  std::vector<float> valid = { 1, 1, 0, 2, 2, 1 };
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBoundingBox, ::testing::ValuesIn(getParameters()));
