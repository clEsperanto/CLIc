


#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestGammaCorrection : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                  backend;
  cle::Device::Pointer         device;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 50, 0, 5, 0, 0, 0, 100, 0, 0, 0, 30, 0, 10, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> output;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestGammaCorrection, execute)
{
  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier3::gamma_correction_func(device, gpu_input, nullptr, 0.5);

  gpu_output->readTo(output.data());
  // assert (np.abs(np.mean(a) - 11.1786) < 0.001)
  EXPECT_LT(std::abs(*std::min_element(output.begin(), output.end())), 0.001);
  EXPECT_LT(std::abs(*std::max_element(output.begin(), output.end())) - 100, 0.001);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestGammaCorrection, ::testing::ValuesIn(getParameters()));
