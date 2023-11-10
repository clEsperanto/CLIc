#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestNonzeroToPixelIndex : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 3 * 2> output;
  std::array<float, 5 * 3 * 2> input;
  std::array<float, 5 * 3 * 2> valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), 0);
    std::fill(valid.begin(), valid.end(), 0);
    for (auto it = input.begin(); it != input.end(); ++it)
    {
      *it = static_cast<float>(rand() % 2);
    }
    for (auto it = input.begin(), it_valid = valid.begin(); (it != input.end()) && (it_valid != valid.end());
         ++it, ++it_valid)
    {
      if (*it != 0)
      {
        *it_valid = static_cast<float>((it_valid - valid.begin()) + 1);
      }
    }
  }
};

TEST_P(TestNonzeroToPixelIndex, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(5, 3, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::set_nonzero_pixels_to_pixelindex_func(device, gpu_input, nullptr, 1);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestNonzeroToPixelIndex, ::testing::ValuesIn(getParameters()));
