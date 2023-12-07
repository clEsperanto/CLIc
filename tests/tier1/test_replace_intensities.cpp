#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestReplaceIntensities : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 1 * 1> intmap;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    float value = 0;
    for (auto it = intmap.begin(); it != intmap.end(); ++it)
    {
      *it = static_cast<float>((int)rand() % 10 + 10);
    }
    for (auto it = input.begin(), it_valid = valid.begin(); (it != input.end()) && (it_valid != valid.end());
         ++it, ++it_valid)
    {
      value = static_cast<float>((int)rand() % 10);
      *it = value;
      *it_valid = static_cast<float>(intmap[value]);
    }
  }
};

TEST_P(TestReplaceIntensities, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(10, 5, 3,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(10, 1, 1,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input1->write(input.data());
  gpu_input2->write(intmap.data());

  auto gpu_output = cle::tier1::replace_values_func(device, gpu_input1, gpu_input2, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestReplaceIntensities, ::testing::ValuesIn(getParameters()));
