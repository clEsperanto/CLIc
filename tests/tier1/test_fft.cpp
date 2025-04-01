#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestFFT : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 1> input = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
  };
};

TEST_P(TestFFT, smoothSize)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);


  auto smooth_size = cle::fft::get_next_smooth({ 10, 5, 1 });
  EXPECT_EQ(smooth_size[0], 10);
  EXPECT_EQ(smooth_size[1], 5);
  EXPECT_EQ(smooth_size[2], 1);

  smooth_size = cle::fft::get_next_smooth({ 11, 4, 7 });
  EXPECT_EQ(smooth_size[0], 12);
  EXPECT_EQ(smooth_size[1], 4);
  EXPECT_EQ(smooth_size[2], 7);

  smooth_size = cle::fft::get_next_smooth({ 25, 30, 2 });
  EXPECT_EQ(smooth_size[0], 25);
  EXPECT_EQ(smooth_size[1], 30);
  EXPECT_EQ(smooth_size[2], 2);

  smooth_size = cle::fft::get_next_smooth({ 10, 10, 11 });
  EXPECT_EQ(smooth_size[0], 10);
  EXPECT_EQ(smooth_size[1], 10);
  EXPECT_EQ(smooth_size[2], 12);

  smooth_size = cle::fft::get_next_smooth({ 10, 10, 10 });
  EXPECT_EQ(smooth_size[0], 10);
  EXPECT_EQ(smooth_size[1], 10);
  EXPECT_EQ(smooth_size[2], 10);
}

TEST_P(TestFFT, executeVKFFT)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  auto gpu_input = cle::Array::create(10, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_final = cle::Array::create(gpu_input);
  gpu_final->fill(0);

  auto gpu_output = cle::fft::performFFT(gpu_input, nullptr);
  cle::fft::performIFFT(gpu_output, gpu_final);

  std::vector<float> output(gpu_final->size());
  gpu_final->readTo(output.data());
  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], input[i], 0.1);
  }
}


TEST_P(TestFFT, executeConvolution)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  std::array<float, 3 * 3 * 1> input = {
    0, 0, 0, 0, 1, 0, 0, 0, 0,
  };
  std::array<float, 3 * 3 * 1> kernel = {
    1, 2, 0, 0, 0, 0, 0, 0, 0,
  };
  std::array<float, 3 * 3 * 1> valid = {
    0, 0, 0, 0, 1, 2, 0, 0, 0,
  };

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_psf = cle::Array::create(gpu_input);
  gpu_psf->writeFrom(kernel.data());

  auto gpu_final = cle::fft::performConvolution(gpu_input, gpu_psf, nullptr, false);

  std::vector<float> output(gpu_final->size());
  gpu_final->readTo(output.data());
  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.1);
  }
}

TEST_P(TestFFT, executeConvolutionCorr)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  std::array<float, 3 * 3 * 1> input = {
    0, 0, 0, 0, 1, 0, 0, 0, 0,
  };
  std::array<float, 3 * 3 * 1> kernel = {
    1, 2, 0, 0, 0, 0, 0, 0, 0,
  };

  std::array<float, 3 * 3 * 1> valid_corr = {
    0, 0, 0, 2, 1, -0, 0, 0, -0,
  };

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_psf = cle::Array::create(gpu_input);
  gpu_psf->writeFrom(kernel.data());

  auto gpu_final = cle::fft::performConvolution(gpu_input, gpu_psf, nullptr, true);

  std::vector<float> output(gpu_final->size());
  gpu_final->readTo(output.data());
  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_corr[i], 0.1);
  }
}

TEST_P(TestFFT, executeConvolutionPadded)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  std::array<float, 13 * 11 * 1> input = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,
    4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,
    7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
    13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
  };
  std::array<float, 3 * 3 * 1> kernel = {
    1, 1, 1, 1, 1, 1, 1, 1, 1,
  };

  // std::array<float, 3 * 3 * 1> valid_corr = {
  //   0, 0, 0, 2, 1, -0, 0, 0, -0,
  // };

  auto gpu_input = cle::Array::create(13, 11, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_psf = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_psf->writeFrom(kernel.data());

  auto gpu_final = cle::tier8::fft_convolution_func(device, gpu_input, gpu_psf, nullptr, false);

  // std::vector<float> output(gpu_final->size());
  // gpu_final->readTo(output.data());
  // EXPECT_EQ(output.size(), input.size());
  // for (size_t i = 0; i < output.size(); i++)
  // {
  //   EXPECT_NEAR(output[i], valid_corr[i], 0.1);
  // }
}

TEST_P(TestFFT, executeDeconvolutionPadded)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  std::array<float, 3 * 3 * 1> valid = {
    0, 0, 0, 0, 3, 0, 0, 0, 0,
  };
  std::array<float, 3 * 3 * 1> kernel = {
    1, 2, 0, 0, 0, 0, 0, 0, 0,
  };
  std::array<float, 3 * 3 * 1> input = {
    0, 0, 0, 0, 1, 2, 0, 0, 0,
  };

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_psf = cle::Array::create(gpu_input);
  gpu_psf->writeFrom(kernel.data());

  auto gpu_output = cle::tier8::fft_deconvolution_func(device, gpu_input, gpu_psf, nullptr, nullptr, 100, 0);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  EXPECT_EQ(output.size(), valid.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.1);
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
