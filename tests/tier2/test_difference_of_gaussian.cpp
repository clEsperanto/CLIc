#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestDifferenceOfGaussian : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 3> output;
  std::array<float, 3 * 3 * 3> input;
  std::array<float, 3 * 3 * 3> valid = {
    static_cast<float>(1.217670321), static_cast<float>(2.125371218), static_cast<float>(1.217670321),
    static_cast<float>(2.125371456), static_cast<float>(3.62864542),  static_cast<float>(2.125371456),
    static_cast<float>(1.217670321), static_cast<float>(2.125371218), static_cast<float>(1.217670321),
    static_cast<float>(2.125371456), static_cast<float>(3.62864542),  static_cast<float>(2.125371456),
    static_cast<float>(3.628645658), static_cast<float>(6.114237785), static_cast<float>(3.628645658),
    static_cast<float>(2.125371456), static_cast<float>(3.62864542),  static_cast<float>(2.125371456),
    static_cast<float>(1.217670321), static_cast<float>(2.125371218), static_cast<float>(1.217670321),
    static_cast<float>(2.125371456), static_cast<float>(3.62864542),  static_cast<float>(2.125371456),
    static_cast<float>(1.217670321), static_cast<float>(2.125371218), static_cast<float>(1.217670321)
  };

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), 0.0f);
    const int center = (3 / 2) + (3 / 2) * 3 + (3 / 2) * 3 * 3;
    input[center] = 100.0f;
  }
};

TEST_P(TestDifferenceOfGaussian, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::difference_of_gaussian_func(device, gpu_input, nullptr, 1, 1, 1, 3, 3, 3);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.0001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDifferenceOfGaussian, ::testing::ValuesIn(getParameters()));
