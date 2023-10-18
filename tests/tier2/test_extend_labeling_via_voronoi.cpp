
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestExtLabelVoronoi : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 6 * 3 * 2> input = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 6 * 3 * 2> valid = { 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1,
                                         2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1 };
  std::array<float, 6 * 3 * 2> output;
};

TEST_P(TestExtLabelVoronoi, execute)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(6, 3, 2, cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier2::extend_labeling_via_voronoi_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestExtLabelVoronoi, ::testing::ValuesIn(getParameters()));