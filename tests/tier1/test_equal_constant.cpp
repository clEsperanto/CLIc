#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"
#include <random>

class TestEqualConstant : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    static std::uniform_int_distribution<int> distribution(1, 10);
    static std::default_random_engine         generator;
    std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
    std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x == 5; });
  }
};

TEST_P(TestEqualConstant, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::equal_constant_func(device, gpu_input, nullptr, 5);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestEqualConstant, ::testing::ValuesIn(getParameters()));
