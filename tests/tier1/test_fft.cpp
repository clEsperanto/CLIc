#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestFFT : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 1> input_2 = {
    1,  2,  3,  4,  5,  6,  7,  8,  9, 10,   
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,  
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
  };
  std::array<float, 10 * 5 * 1> input_3 = {
    10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0,0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,0,
  };
};

TEST_P(TestFFT, executeCLFFT)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
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


TEST_P(TestFFT, executeVKFFT)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2.data());
  auto gpu_final = cle::Array::create(gpu_input);
  gpu_final->fill(0);

  cle::print<float>(gpu_input, "input real");

  auto output = cle::fft::performFFT(gpu_input, nullptr);
  cle::print<float>(output, "output complex");

  cle::fft::performIFFT(output, gpu_final);
  cle::print<float>(gpu_final, "output real");
}


TEST_P(TestFFT, executeConvolution)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2.data());

  auto gpu_psf = cle::Array::create(gpu_input);
  gpu_psf->writeFrom(input_3.data());

  cle::print<float>(gpu_input, "input");
  cle::print<float>(gpu_psf, "kernel");

  auto gpu_final = cle::fft::performConvolution(gpu_input, gpu_psf, nullptr, false);
  
  cle::print<float>(gpu_final, "output");
}


TEST_P(TestFFT, executeConvolutionCorrelated)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2.data());

  auto gpu_psf = cle::Array::create(gpu_input);
  gpu_psf->writeFrom(input_3.data());

  auto gpu_final = cle::fft::performConvolution(gpu_input, gpu_psf, nullptr, true);
  
  cle::print<float>(gpu_final, "output");
}


TEST_P(TestFFT, executeDeconvolution)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);


  std::array<float, 9 * 1 * 1> input = {
    0, 2, 2, 0, 2, 2, 2, 0, 0
  };
  std::array<float, 9 * 1 * 1> psf = {
    2, 0, 0, 0, 0, 0, 0, 0, 0 
  };


  auto gpu_input = cle::Array::create(9, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_psf = cle::Array::create(gpu_input);
  gpu_psf->writeFrom(psf.data());

  auto gpu_estimate = cle::Array::create(gpu_input);
  gpu_estimate->writeFrom(input.data());

  auto gpu_normal = cle::Array::create(gpu_input);
  gpu_normal->fill(1);

  cle::fft::performDeconvolution(gpu_input, gpu_psf, gpu_normal, gpu_estimate, 1, 0);
  
  cle::print<float>(gpu_estimate, "output");
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
