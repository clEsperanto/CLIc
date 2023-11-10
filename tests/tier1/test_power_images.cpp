#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestPowerImages : public ::testing::TestWithParam<std::string>
{
protected:
  const float                   value = 2;
  const float                   exp = 4;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    std::fill(input1.begin(), input1.end(), static_cast<float>(value));
    std::fill(input2.begin(), input2.end(), static_cast<float>(exp));
    std::fill(valid.begin(), valid.end(), static_cast<float>(std::pow(value, exp)));
  }
};

TEST_P(TestPowerImages, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input1 = cle::Array::create(10, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::power_images_func(device, gpu_input1, gpu_input2, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestPowerImages, ::testing::ValuesIn(getParameters()));
