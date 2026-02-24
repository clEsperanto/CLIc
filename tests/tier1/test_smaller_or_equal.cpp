#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"
#include <random>

class TestSmallerEqual : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    static std::uniform_int_distribution<int> distribution(1, 10);
    static std::default_random_engine         generator;
    std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
    std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
    std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a <= b; });
  }
};

TEST_P(TestSmallerEqual, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::smaller_or_equal_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSmallerEqual, ::testing::ValuesIn(getParameters()));
