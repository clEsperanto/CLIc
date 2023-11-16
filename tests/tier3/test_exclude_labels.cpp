#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestExcludeLabels : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 7 * 6 * 3> output;
  std::array<uint32_t, 7 * 6 * 3> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 0, 7, 0, 0, 0, 1, 0, 0, 7, 5, 5,
                                            8, 8, 8, 0, 0, 0, 0, 0, 4, 4, 9, 3, 0, 0, 0, 4, 4, 6, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestExcludeLabels, onList)
{
  std::array<uint32_t, 10 * 1 * 1> list = { 0, 0, 0, 1, 0, 0, 1, 0, 1, 0 };

  std::array<uint32_t, 7 * 6 * 3> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 0, 5, 0, 0, 0, 1, 0, 0, 5, 4, 4,
                                            0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 6, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(7, 6, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  auto gpu_list = cle::Array::create(10, 1, 1, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());
  gpu_list->write(list.data());

  auto gpu_output = cle::tier3::exclude_labels_func(device, gpu_input, gpu_list, nullptr);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestExcludeLabels, onEdges)
{
  std::array<uint32_t, 7 * 6 * 3> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(7, 6, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier3::exclude_labels_on_edges_func(device, gpu_input, nullptr, true, true, true);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestExcludeLabels, ::testing::ValuesIn(getParameters()));
