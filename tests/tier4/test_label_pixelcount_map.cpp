#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLabelPixelCountMap : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 5 * 1> input = { 1, 1, 2, 0, 3, 3, 1, 1, 2, 0, 3, 3, 0, 0, 0,
                                            0, 0, 0, 4, 4, 5, 6, 6, 6, 4, 4, 5, 6, 6, 6 };

  std::array<float, 6 * 5 * 1> valid = { 4, 4, 2, 0, 4, 4, 4, 4, 2, 0, 4, 4, 0, 0, 0,
                                         0, 0, 0, 4, 4, 2, 6, 6, 6, 4, 4, 2, 6, 6, 6 };
};

TEST_P(TestLabelPixelCountMap, execute2d)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::pixel_count_map_func(device, gpu_input, nullptr);

  std::vector<float> output(gpu_output->size());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelPixelCountMap, ::testing::ValuesIn(getParameters()));
