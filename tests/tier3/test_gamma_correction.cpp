


#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestExistingLabels : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> input = {
    0, 0, 0, 0, 0, 0, 50, 0, 5, 0, 0, 0, 100, 0, 0, 0, 30, 0, 10, 0, 0, 0, 0, 0, 0
  };
  std::array<float, 5 * 5 * 1> output;
};

TEST_P(TestExistingLabels, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier3::gamma_correction_func(device, gpu_input, nullptr, 0.5);

  gpu_output->readTo(output.data());
  // assert (np.abs(np.mean(a) - 11.1786) < 0.001)
  EXPECT_LT(std::abs(*std::min_element(output.begin(), output.end())), 0.001);
  EXPECT_LT(std::abs(*std::max_element(output.begin(), output.end())) - 100, 0.001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestExistingLabels, ::testing::ValuesIn(getParameters()));
