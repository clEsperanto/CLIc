#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMask : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> mask;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    for (size_t i = 0; i < input.size(); ++i)
    {
      input[i] = static_cast<float>(rand() % 10);
    }
    std::fill(mask.begin(), mask.end(), static_cast<float>(0));
    std::fill(valid.begin(), valid.end(), static_cast<float>(0));
    const int center = (10 / 2) + (5 / 2) * 10 + (3 / 2) * 10 * 5;
    valid[center] = input[center];
    mask[center] = static_cast<float>(1);
  }
};

TEST_P(TestMask, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_mask = cle::Array::create(gpu_input);
  gpu_input->write(input.data());
  gpu_mask->write(mask.data());

  auto gpu_output = cle::tier1::mask_func(device, gpu_input, gpu_mask, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMask, ::testing::ValuesIn(getParameters()));
