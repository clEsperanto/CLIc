#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLabeling : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 5 * 3 * 2> valid_diamond = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                          2, 0, 0, 1, 0, 0, 3, 0, 1, 1, 0, 3, 0, 0, 0 };
  const std::array<uint32_t, 5 * 3 * 2> valid_box = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                      2, 0, 0, 1, 0, 0, 2, 0, 1, 1, 0, 2, 0, 0, 0 };
  const std::array<uint32_t, 5 * 3 * 2> input = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                  1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0 };
  std::array<uint32_t, 5 * 3 * 2>       output;
};

TEST_P(TestLabeling, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier5::connected_components_labeling_func(device, gpu_input, nullptr, "box");

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestLabeling, executeDiamond)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier5::connected_components_labeling_func(device, gpu_input, nullptr, "diamond");

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_diamond[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabeling, ::testing::ValuesIn(getParameters()));
