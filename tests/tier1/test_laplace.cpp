#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLaplace : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 3 * 2> input_box = { 1, 0, 2, 1, 0, 1, 0, 3, 0, 2, 0, 1, 3, 0, 0, 1, 0, 1, 1, 0, 2, 0, 2, 1 };

  std::array<float, 4 * 3 * 2> valid_box = { 4,  -22, 30,  -6, -21, 6, -26, 50,  -19, 34,  -28, -2,
                                             50, -23, -18, 3,  -27, 3, 5,   -26, 31,  -25, 28,  -1 };


  std::array<float, 5 * 5 * 1> input_sphere = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<float, 5 * 5 * 1> valid_sphere = { 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 4, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestLaplace, executeDeprecatedBox)
{
  std::array<float, 4 * 3 * 2> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_box.data());

  auto gpu_output = cle::tier1::laplace_box_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}


TEST_P(TestLaplace, executeDeprecatedSphere)
{
  std::array<float, 5 * 5 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_sphere.data());

  auto gpu_output = cle::tier1::laplace_diamond_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_sphere[i]);
  }
}


TEST_P(TestLaplace, executeBox)
{
  std::array<float, 4 * 3 * 2> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_box.data());

  auto gpu_output = cle::tier1::laplace_func(device, gpu_input, nullptr, "box");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestLaplace, executeSphere)
{
  std::array<float, 5 * 5 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_sphere.data());

  auto gpu_output = cle::tier1::laplace_func(device, gpu_input, nullptr, "sphere");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_sphere[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLaplace, ::testing::ValuesIn(getParameters()));
