
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestNonzeroMaximumBox : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(0));
    std::fill(valid.begin(), valid.end(), static_cast<float>(0));
    const size_t center = (10 / 2) + (5 / 2) * 10 + (3 / 2) * 5 * 10;
    input[center] = 100;
    valid[center] = 100;
    for (size_t i = 0; i < input.size(); ++i)
    {
      if ((i == center - 1 && i == center + 1) || (i == center + 10) || (i == center - 10) ||
          (i == center - (5 * 10)) || (i == center + (5 * 10)))
      {
        input[i] = static_cast<float>(rand() % 10 + 2);
        valid[i] = static_cast<float>(100);
      }
    }
  }
};

TEST_P(TestNonzeroMaximumBox, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(10, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());
  auto flag = cle::Array::create(1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  flag->fill(1);

  auto gpu_output = cle::tier1::nonzero_maximum_box_func(device, gpu_input, flag, nullptr);

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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestNonzeroMaximumBox, ::testing::ValuesIn(getParameters()));
