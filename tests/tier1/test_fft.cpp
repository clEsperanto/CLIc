#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestFFT : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 1> input_2 = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
  };
};

TEST_P(TestFFT, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "cpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2.data());
  cle::print<float>(gpu_input, "input");

  auto gpu_output = cle::Array::create(10, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_output->fill(0);

  // Perform FFT and get the output complex buffer
  auto gpu_complex = cle::fft::fft_forward(gpu_input, nullptr);
  std::cout << "complexe shape : " << gpu_complex->width() << " " << gpu_complex->height() << " "
            << gpu_complex->depth() << std::endl;
  cle::print<float>(gpu_complex, "complex");

  // Perform IFFT and store the result in a real buffer
  cle::fft::fft_backward(gpu_complex, gpu_output);
  cle::print<float>(gpu_output, "output");

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());

  EXPECT_EQ(output.size(), input_2.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], input_2[i], 0.1);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestFFT, ::testing::ValuesIn(getParameters()));
