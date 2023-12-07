#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestDetectMinima : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> valid;
  std::array<uint8_t, 10 * 5 * 3> input;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<uint8_t>(100));
    std::fill(valid.begin(), valid.end(), static_cast<uint8_t>(0));
    const size_t center = (10 / 2) + (5 / 2) * 10 + (3 / 2) * 10 * 5;
    input[center] = static_cast<uint8_t>(42);
    valid[center] = static_cast<uint8_t>(1);
  }
};

TEST_P(TestDetectMinima, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3,3 , cle::dType::UINT8, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::detect_minima_box_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDetectMinima, ::testing::ValuesIn(getParameters()));
