
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestThresholdOtsu : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 6.0058594 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };    // 6.00392 clic
  std::array<uint8_t, 3 * 2 * 2>       output;


  const std::array<float, 3 * 3 * 1>   input_low = { 0, 0, 0, 0, 0.003, 0, 0, 0, 0 };
  const std::array<uint8_t, 3 * 3 * 1> valid_low = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
  std::array<uint8_t, 3 * 3 * 1>       output_low;


const std::vector<float> test = {  
0.2295937 , 0.92521435, 1.8537747 , 1.8731794 , 0.9393289 , 0.22589178,
0.3740187 , 1.3714234 , 2.6094873 , 2.590501  , 1.3086381 ,0.32493836,
0.2948968 , 0.9629241 , 1.6921631 , 1.6221197 , 0.826933  ,0.21854304,
0.1166786 , 0.36253795, 0.68537563, 0.8279946 , 0.6108529 ,0.26835373,
0.04627159, 0.25866497, 0.8760503 , 1.6040901 , 1.5428433 ,0.8239908 ,
0.0561676 , 0.3889102 , 1.3680687 , 2.4913776 , 2.392372  ,1.2908775 ,
0.06311502, 0.3839612 , 1.1596216 , 1.9070312 , 1.7505941 ,0.9107878 };

const std::vector<float> valid_test = { 
0, 0, 1, 1, 0, 0,
0, 1, 1, 1, 1, 0,
0, 1, 1, 1, 0, 0,
0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 1, 0,
0, 0, 1, 1, 1, 1,
0, 0, 1, 1, 1, 0};


};

TEST_P(TestThresholdOtsu, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestThresholdOtsu, lowIntensity)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_low.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  gpu_output->readTo(output_low.data());
  for (int i = 0; i < output_low.size(); i++)
  {
    EXPECT_EQ(output_low[i], valid_low[i]);
  }
}


TEST_P(TestThresholdOtsu, gauss)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 7, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(test.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  std::vector<uint8_t> output_test(gpu_output->size());
    gpu_output->readTo(output_test.data());
  for (int i = 0; i < output_test.size(); i++)
  {
    EXPECT_EQ(output_test[i], valid_test[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestThresholdOtsu, ::testing::ValuesIn(getParameters()));
