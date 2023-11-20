#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLabelOperations : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 6 * 2> output;
};

TEST_P(TestLabelOperations, executeDilate)
{
  std::array<uint32_t, 6 * 6 * 2> input = { 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
                                            0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0 };
  std::array<uint32_t, 6 * 6 * 2> dilated = { 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2, 0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 4, 0,
                                              5, 5, 4, 4, 4, 3, 5, 5, 0, 0, 3, 3, 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2,
                                              0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 4, 0, 5, 5, 4, 4, 4, 3, 5, 5, 0, 0, 3, 3 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier6::dilate_labels_func(device, gpu_input, nullptr, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], dilated[i]);
  }

  cle::tier6::dilate_labels_func(device, gpu_input, gpu_output, 0);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]);
  }
}

TEST_P(TestLabelOperations, executeErode)
{
  std::array<uint32_t, 6 * 6 * 2> input = { 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2, 0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 4, 0,
                                            5, 5, 4, 4, 4, 3, 5, 5, 0, 0, 3, 3, 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2,
                                            0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 4, 0, 5, 5, 4, 4, 4, 3, 5, 5, 0, 0, 3, 3 };
  std::array<uint32_t, 6 * 6 * 2> eroded = { 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
                                             0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier6::erode_labels_func(device, gpu_input, nullptr, 1, true);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], eroded[i]);
  }

  cle::tier6::erode_labels_func(device, gpu_input, gpu_output, 0, true);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]);
  }
}

TEST_P(TestLabelOperations, executeOpening)
{
  std::array<uint32_t, 6 * 6 * 2> input = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 0, 1, 1, 1, 2, 2, 0,
                                            0, 0, 0, 2, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                            1, 1, 1, 2, 2, 0, 1, 1, 1, 2, 2, 0, 0, 0, 0, 2, 2, 0, 3, 0, 0, 0, 0, 0 };
  std::array<uint32_t, 6 * 6 * 2> opened = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                             1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier7::opening_labels_func(device, gpu_input, nullptr, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], opened[i]);
  }

  cle::tier7::opening_labels_func(device, gpu_input, gpu_output, 0);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]);
  }
}

TEST_P(TestLabelOperations, executeClosing)
{
  std::array<uint32_t, 6 * 6 * 2> input = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0,
                                            0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                            1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0 };
  std::array<uint32_t, 6 * 6 * 2> closed = { 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 0, 0, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2,
                                             3, 3, 0, 2, 2, 2, 3, 0, 0, 0, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 0, 0,
                                             1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 3, 3, 0, 2, 2, 2, 3, 0, 0, 0, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier7::closing_labels_func(device, gpu_input, nullptr, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], closed[i]);
  }

  cle::tier7::closing_labels_func(device, gpu_input, gpu_output, 0);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelOperations, ::testing::ValuesIn(getParameters()));