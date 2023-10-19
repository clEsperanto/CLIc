#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMaximumImages : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    const float value1 = static_cast<float>(rand() % 4) + 1;
    const float value2 = static_cast<float>(rand() % 2) + 1;
    std::fill(input1.begin(), input1.end(), static_cast<float>(value1));
    std::fill(input2.begin(), input2.end(), static_cast<float>(value2));
    std::fill(valid.begin(), valid.end(), static_cast<float>(std::max(value1, value2)));
  }
};

TEST_P(TestMaximumImages, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input1 = cle::Array::create(10, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::maximum_images_func(device, gpu_input1, gpu_input2, nullptr);

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

#ifdef USE_OPENCL
  parameters.push_back("opencl");
#endif

#ifdef USE_CUDA
  parameters.push_back("cuda");
#endif

  return parameters;
}

INSTANTIATE_TEST_CASE_P(InstantiationName, TestMaximumImages, ::testing::ValuesIn(getParameters()));
