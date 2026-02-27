#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>
#include <random>

class TestReplaceIntensity : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    static std::uniform_int_distribution<int> distribution(1, 10);
    static std::default_random_engine         generator;
    std::generate(input.begin(), input.end(), []() { return static_cast<float>(distribution(generator)); });
    std::transform(input.begin(), input.end(), valid.begin(), [](const float & x) {
      if (x == 5)
      {
        return static_cast<float>(100);
      }
      else
      {
        return x;
      }
    });
  }
};

TEST_P(TestReplaceIntensity, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::replace_value_func(device, gpu_input, nullptr, 5, 100);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestReplaceIntensity, ::testing::ValuesIn(getParameters()));
