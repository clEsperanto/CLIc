#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestCountNeighbor : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 1 * 1> output;
  std::array<float, 5 * 5 * 1>    input = { 1, 1, 0, 3, 3, 1, 1, 2, 3, 3, 0, 2, 2, 2, 0, 4, 4, 2, 5, 5, 4, 4, 0, 5, 5 };
};

TEST_P(TestCountNeighbor, ignoreBG)
{
  std::array<uint32_t, 6 * 1 * 1> valid = { 0, 1, 4, 1, 1, 1 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto touch_mat = cle::tier3::generate_touch_matrix_func(device, gpu_input, nullptr);
  auto gpu_output = cle::tier2::count_touching_neighbors_func(device, touch_mat, nullptr, true);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestCountNeighbor, includeBG)
{
  std::array<uint32_t, 6 * 1 * 1> valid = { 5, 2, 5, 2, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto touch_mat = cle::tier3::generate_touch_matrix_func(device, gpu_input, nullptr);
  auto gpu_output = cle::tier2::count_touching_neighbors_func(device, touch_mat, nullptr, false);

  gpu_output->readTo(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCountNeighbor, ::testing::ValuesIn(getParameters()));
