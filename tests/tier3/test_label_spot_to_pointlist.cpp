#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLabelSpotToPointList : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1>    input = { 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 4 };
  std::array<uint32_t, 4 * 2 * 1> valid = { 1, 2, 3, 4, 1, 3, 1, 4 };
  std::array<uint32_t, 4 * 2 * 1> output;
};

TEST_P(TestLabelSpotToPointList, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier3::labelled_spots_to_pointlist_func(device, gpu_input, nullptr);

  std::cout << gpu_output << std::endl;
  gpu_output->read(output.data());
  for (auto && i : output)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;


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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelSpotToPointList, ::testing::ValuesIn(getParameters()));
