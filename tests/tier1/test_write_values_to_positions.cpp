#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestWriteValuesTopositions : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 6 * 7 * 1> output_2d;
  std::array<float, 5 * 3 * 1> list_2d = { 0, 0, 2, 3, 5, 0, 1, 3, 2, 6, 8, 7, 6, 5, 4 };
  std::array<float, 6 * 7 * 1> valid_2d = { 8, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 6,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 };
  std::array<float, 6 * 7 * 2> output_3d;
  std::array<float, 5 * 4 * 1> list_3d = { 0, 0, 2, 3, 5, 0, 1, 3, 2, 6, 0, 0, 0, 0, 1, 8, 7, 6, 5, 4 };
  std::array<float, 6 * 7 * 2> valid_3d = { 8, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 6,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 };
};

TEST_P(TestWriteValuesTopositions, execute2D)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_coord = cle::Array::create(5, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_coord->writeFrom(list_2d.data());

  auto gpu_output = cle::tier1::write_values_to_positions_func(device, gpu_coord, nullptr);

  gpu_output->readTo(output_2d.data());
  for (int i = 0; i < output_2d.size(); i++)
  {
    EXPECT_EQ(output_2d[i], valid_2d[i]);
  }
}

TEST_P(TestWriteValuesTopositions, execute3D)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_coord = cle::Array::create(5, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_coord->writeFrom(list_3d.data());

  auto gpu_output = cle::tier1::write_values_to_positions_func(device, gpu_coord, nullptr);

  gpu_output->readTo(output_3d.data());
  for (int i = 0; i < output_3d.size(); i++)
  {
    EXPECT_EQ(output_3d[i], valid_3d[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestWriteValuesTopositions, ::testing::ValuesIn(getParameters()));
