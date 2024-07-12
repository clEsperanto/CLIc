
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestNonzeroMaximum : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;

  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 2, 2, 3, 0, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0 };

  std::array<float, 5 * 5 * 1> valid_box = {
    0, 0, 0, 0, 0, 0, 2, 3, 3, 0, 0, 3, 4, 4, 0, 0, 3, 4, 4, 0, 0, 0, 0, 0, 0
  };

  std::array<float, 5 * 5 * 1> valid_diam = {
    0, 0, 0, 0, 0, 0, 2, 2, 3, 0, 0, 3, 3, 4, 0, 0, 3, 4, 4, 0, 0, 0, 0, 0, 0
  };
};

TEST_P(TestNonzeroMaximum, executeDeprecatedBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);
  auto gpu_output = cle::tier1::nonzero_maximum_box_func(device, gpu_input, flag, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestNonzeroMaximum, executeDeprecatedSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);

  auto gpu_output = cle::tier1::nonzero_maximum_diamond_func(device, gpu_input, flag, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_diam[i]);
  }
}


TEST_P(TestNonzeroMaximum, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);
  auto gpu_output = cle::tier1::nonzero_maximum_func(device, gpu_input, flag, nullptr, "box");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestNonzeroMaximum, executeSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);

  auto gpu_output = cle::tier1::nonzero_maximum_func(device, gpu_input, flag, nullptr, "sphere");

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestNonzeroMaximum, ::testing::ValuesIn(getParameters()));
