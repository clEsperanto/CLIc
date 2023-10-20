
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMinMaskPixel : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 3 * 2> input;
  std::array<float, 5 * 3 * 2> mask;
  float                        valid = 3;

  virtual void
  SetUp()
  {
    for (size_t i = 0; i < input.size(); i++)
    {
      input[i] = 100;
      mask[i] = (i % 2 == 0) ? 1 : 0;
    }
    input[2] = 3;
  }
};

TEST_P(TestMinMaskPixel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(5, 3, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_mask = cle::Array::create(gpu_input);
  gpu_input->write(input.data());
  gpu_mask->write(mask.data());

  auto output = cle::tier2::minimum_of_masked_pixels_func(device, gpu_input, gpu_mask);

  EXPECT_EQ(output, valid);
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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestMinMaskPixel, ::testing::ValuesIn(getParameters()));