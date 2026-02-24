#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>


class TestDeskew : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestDeskew, deskew_y)
{

  if (!device->supportImage())
  {
    GTEST_SKIP() << "Device does not support image objects.";
  }

  if (device->vendorName().find("POCL") != std::string::npos)
  {
    GTEST_SKIP() << "POCL does not reliably support image interpolation for deskew operations.";
  }

  auto coord_to_index = [](size_t x, size_t y, size_t z, size_t width, size_t height) -> size_t {
    return z * height * width + y * width + x;
  };

  std::array<float, 10 * 10 * 10> input = { 0.0f };
  auto                            idx = coord_to_index(1, 1, 1, 10, 10);
  input[idx] = 1.0f;

  std::array<float, 10 * 19 * 5> valid = { 0.0f };
  idx = coord_to_index(1, 2, 3, 10, 19);
  valid[idx] = 0.169872939f;

  std::array<float, 10 * 19 * 5> output;

  cle::Array::Pointer gpu_input = nullptr;
  try
  {
    gpu_input = cle::Array::create(10, 10, 10, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);
  }
  catch (const std::runtime_error & e)
  {
    GTEST_SKIP() << "Device does not support image objects.";
  }
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier7::deskew_y_func(device, gpu_input, nullptr, 30, 1.0f, 1.0f, 1.0f, 1.0f);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.00001);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDeskew, ::testing::ValuesIn(getParameters()));
