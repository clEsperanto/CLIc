#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestGenerateDistanceMatrix : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 4 };
  std::array<float, 5 * 5 * 1> valid = { 0,         0,         0,        0,         0,        0,         0., 2.236068, 2,
                                         4.2426405, 0,         2.236068, 0,         2.236068, 2.236068,  0,  2,        2.236068,
                                         0,         3.1622777, 0,        4.2426405, 2.236068, 3.1622777, 0 };
};

TEST_P(TestGenerateDistanceMatrix, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto pts_list = cle::tier3::labelled_spots_to_pointlist_func(device, gpu_input, nullptr);
  auto gpu_output = cle::tier1::generate_distance_matrix_func(device, pts_list, pts_list, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestGenerateDistanceMatrix, ::testing::ValuesIn(getParameters()));
