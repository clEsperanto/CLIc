#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestPaste : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int16_t, 5 * 5 * 1> output;
  std::array<int16_t, 5 * 5 * 1> input = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
  std::array<int16_t, 5 * 5 * 1> valid = { 4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4, 4 };
};

TEST_P(TestPaste, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::INT16, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_image_borders_func(device, gpu_input, 4);

  gpu_input->read(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestPaste, ::testing::ValuesIn(getParameters()));
