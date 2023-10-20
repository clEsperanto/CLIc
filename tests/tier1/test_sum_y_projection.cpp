#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestSumProjectionY : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 1 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 1 * 3> valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(1));
    std::fill(valid.begin(), valid.end(), static_cast<float>(5));
  }
};

TEST_P(TestSumProjectionY, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(10, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::sum_y_projection_func(device, gpu_input, nullptr);

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

#ifdef USE_OPENCL
  parameters.push_back("opencl");
#endif

#ifdef USE_CUDA
  parameters.push_back("cuda");
#endif

  return parameters;
}

INSTANTIATE_TEST_CASE_P(InstantiationName, TestSumProjectionY, ::testing::ValuesIn(getParameters()));