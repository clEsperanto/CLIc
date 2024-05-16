#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestHistogram : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10>         valid;
  std::array<uint32_t, 10>      output;

  virtual void
  SetUp()
  {
    std::fill(valid.begin(), valid.end(), 0.0f);
    for (size_t i = 0; i < input.size(); i++)
    {
      input[i] = i % 10;
    }
    for (auto && i : input)
    {
      valid[i] += static_cast<float>(1);
    }
  }
};

TEST_P(TestHistogram, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier3::histogram_func(device, gpu_input, nullptr, 10);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestHistogram, ::testing::ValuesIn(getParameters()));
