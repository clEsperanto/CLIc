#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>


class TestDeskew : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestDeskew, deskew_y)
{
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

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");

  if (!device->supportImage())
  {
    GTEST_SKIP() << "Device does not support image objects.";
  }


  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 10, 10, 3, cle::dType::FLOAT, cle::mType::IMAGE, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier7::deskew_y_func(device, gpu_input, nullptr, 30, 1.0f, 1.0f, 1.0f, 1.0f);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.00001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDeskew, ::testing::ValuesIn(getParameters()));
