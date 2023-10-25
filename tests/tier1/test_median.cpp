#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMedian : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 0, 0, 9, 9, 9, 0, 0, 9, 9, 9, 0, 0,
  };
  std::array<float, 5 * 5 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 0, 0, 9, 9, 9, 0, 0, 9, 9, 9, 0, 0 };
};

TEST_P(TestMedian, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::median_box_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// TEST_P(TestMedian, executeDiamond)
// {
//   std::string param = GetParam();
//   cle::BackendManager::getInstance().setBackend(param);
//   auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

//   auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
//   gpu_input->write(input.data());

//   auto gpu_output = cle::tier1::median_diamond_func(device, gpu_input, nullptr, 1, 1, 0);

//   gpu_output->read(output.data());
//   for (int i = 0; i < output.size(); i++)
//   {
//     EXPECT_EQ(output[i], valid[i]);
//   }
// }

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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestMedian, ::testing::ValuesIn(getParameters()));
