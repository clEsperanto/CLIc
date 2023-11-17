#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestCropBorder : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 4 * 1> input_2d = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1 };

  std::array<float, 4 * 4 * 4> input_3d = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1,

                                            0, 0, 0, 1, 0, 1, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1,

                                            0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1,

                                            0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1 };
};

TEST_P(TestCropBorder, execute2D)
{
  std::array<float, 2 * 2 * 1> valid = { 0, 3, 0, 3 };
  std::array<float, 2 * 2 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 4, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input_2d.data());

  auto gpu_output = cle::tier2::crop_border_func(device, gpu_input, nullptr, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestCropBorder, execute3D)
{
  std::array<float, 2 * 2 * 2> valid = { 1, 3, 0, 3, 0, 3, 0, 3 };
  std::array<float, 2 * 2 * 2> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 4, 4, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input_3d.data());

  auto gpu_output = cle::tier2::crop_border_func(device, gpu_input, nullptr, 1);

  gpu_output->read(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCropBorder, ::testing::ValuesIn(getParameters()));
