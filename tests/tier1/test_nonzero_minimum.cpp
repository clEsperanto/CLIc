
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestNonzeroMinimum : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;

  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 2, 2, 3, 0, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0 };

  std::array<float, 5 * 5 * 1> valid_box = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0
  };

  std::array<float, 5 * 5 * 1> valid_diam = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 2, 0, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0
  };
};


TEST_P(TestNonzeroMinimum, executeDeprecatedBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);
  auto gpu_output = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_output->fill(0);

  cle::tier1::nonzero_minimum_box_func(device, gpu_input, flag, gpu_output);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestNonzeroMinimum, executeDeprecatedDiamond)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);
  auto gpu_output = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_output->fill(0);

  cle::tier1::nonzero_minimum_diamond_func(device, gpu_input, flag, gpu_output);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_diam[i]);
  }
}


TEST_P(TestNonzeroMinimum, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);
  auto gpu_output = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_output->fill(0);

  cle::tier1::nonzero_minimum_func(device, gpu_input, flag, gpu_output, "box");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestNonzeroMinimum, executeSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);
  auto gpu_output = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_output->fill(0);

  cle::tier1::nonzero_minimum_func(device, gpu_input, flag, gpu_output, "sphere");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_diam[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestNonzeroMinimum, ::testing::ValuesIn(getParameters()));
