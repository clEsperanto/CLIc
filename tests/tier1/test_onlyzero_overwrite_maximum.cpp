#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestOnlyzeroOverwriteMaximum : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 2> output;

  std::array<float, 5 * 5 * 2> input = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 2, 3, 4, 0, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<float, 5 * 5 * 2> valid_box = {
    1, 2, 3, 3, 3, 2, 1, 2, 3, 4, 4, 2, 3, 4, 5, 4, 4, 4, 5, 5, 4, 4, 5, 5, 5,
    1, 2, 3, 3, 3, 2, 3, 4, 4, 4, 4, 4, 5, 5, 5, 4, 4, 5, 5, 5, 4, 4, 5, 5, 5
  };

  std::array<float, 5 * 5 * 2> valid_sphere = { 0, 1, 2, 3, 0, 1, 1, 2, 3, 3, 2, 2, 3, 4, 4, 4, 4,
                                                4, 5, 5, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3,
                                                0, 0, 2, 3, 4, 0, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestOnlyzeroOverwriteMaximum, executeDeprecatedBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(0);

  auto gpu_output = cle::tier1::onlyzero_overwrite_maximum_box_func(device, gpu_input, flag, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestOnlyzeroOverwriteMaximum, executeDeprecatedSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(0);

  auto gpu_output = cle::tier1::onlyzero_overwrite_maximum_diamond_func(device, gpu_input, flag, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_sphere[i]);
  }
}

TEST_P(TestOnlyzeroOverwriteMaximum, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(0);

  auto gpu_output = cle::tier1::onlyzero_overwrite_maximum_func(device, gpu_input, flag, nullptr, "box");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestOnlyzeroOverwriteMaximum, executeSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto flag = cle::Array::create(1, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(0);

  auto gpu_output = cle::tier1::onlyzero_overwrite_maximum_func(device, gpu_input, flag, nullptr, "sphere");

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestOnlyzeroOverwriteMaximum, ::testing::ValuesIn(getParameters()));
