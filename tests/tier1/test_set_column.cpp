#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestSetColumn : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    for (auto it = input.begin(), it_valid = valid.begin(); (it != input.end()) && (it_valid != valid.end());
         ++it, ++it_valid)
    {
      *it = static_cast<float>((int)rand() % 10);
      *it_valid = *it;
      if ((it - input.begin()) % 10 == 1)
      {
        *it_valid = static_cast<float>(100);
      }
    }
  }
};

TEST_P(TestSetColumn, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  cle::tier1::set_column_func(device, gpu_input, 1, 100);

  gpu_input->readTo(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSetColumn, ::testing::ValuesIn(getParameters()));
