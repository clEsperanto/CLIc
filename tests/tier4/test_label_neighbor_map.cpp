#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLabelNeighbortMap : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 4 * 4 * 1> label = { 
        1, 1, 2, 2,
        1, 0, 0, 2,
        3, 0, 0, 4,
        3, 3, 4, 4
   };
    std::array<float, 4 * 4 * 1> intensity = { 
        1, 1, 2, 2,
        1, 0, 0, 2,
        4, 0, 0, 6,
        4, 4, 6, 6
   };

  std::array<float, 4 * 4 * 1> valid_mean = { 
        2.3333, 2.3333, 3, 3,
        2.3333, 0, 0, 3,
        3.6666, 0, 0, 4,
        3.6666, 3.6666, 4, 4
   };

    std::array<float, 4 * 4 * 1> valid_max = { 
        4, 4, 6, 6,
        4, 0, 0, 6,
        6, 0, 0, 6,
        6, 6, 6, 6
   };

    std::array<float, 4 * 4 * 1> valid_min = { 
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 0, 0, 2,
        1, 1, 2, 2
   };

  std::array<float, 4 * 4 * 1> valid_std = { 
        0.9660918, 0.9660918, 1.67332, 1.67332,
        0.9660918, 0, 0, 1.67332,
        1.5916449, 0, 0, 1.264911,
        1.5916449, 1.5916449, 1.264911, 1.264911
   };
};

TEST_P(TestLabelNeighbortMap, mean)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_label = cle::Array::create(4, 4, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_label->writeFrom(label.data());
  auto gpu_intensity = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());

  auto gpu_output = cle::tier4::mean_of_touching_neighbors_map_func(device, gpu_intensity, gpu_label, nullptr, 1, true);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_mean[i], 0.0001);
  }
}

TEST_P(TestLabelNeighbortMap, maximum)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_label = cle::Array::create(4, 4, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_label->writeFrom(label.data());
  auto gpu_intensity = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());

  auto gpu_output = cle::tier4::maximum_of_touching_neighbors_map_func(device, gpu_intensity, gpu_label, nullptr, 1, true);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_max[i], 0.0001);
  }
}


TEST_P(TestLabelNeighbortMap, minimum)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_label = cle::Array::create(4, 4, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_label->writeFrom(label.data());
  auto gpu_intensity = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());

  auto gpu_output = cle::tier4::minimum_of_touching_neighbors_map_func(device, gpu_intensity, gpu_label, nullptr, 1, true);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_min[i], 0.0001);
  }
}

TEST_P(TestLabelNeighbortMap, standard_deviation)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_label = cle::Array::create(4, 4, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_label->writeFrom(label.data());
  auto gpu_intensity = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());

  auto gpu_output = cle::tier4::standard_deviation_of_touching_neighbors_map_func(device, gpu_intensity, gpu_label, nullptr, 1, true);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_std[i], 0.0001);
  }
}

TEST_P(TestLabelNeighbortMap, mode)
{

  std::array<uint32_t, 4 * 4 * 1> label = { 
        0, 0, 2, 5,
        0, 3, 1, 2,
        0, 4, 3, 0,
        0, 0, 0, 0
   };
    std::array<float, 4 * 4 * 1> intensity = { 
        0, 0, 2, 5,
        0, 3, 1, 2,
        0, 4, 3, 0,
        0, 0, 0, 0
   };
  std::array<float, 4 * 4 * 1> valid_mode = { 
        0, 0, 1, 2,
        0, 1, 2, 1,
        0, 3, 1, 0,
        0, 0, 0, 0
   };


  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_label = cle::Array::create(4, 4, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_label->writeFrom(label.data());
  auto gpu_intensity = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());

  auto gpu_output = cle::tier4::mode_of_touching_neighbors_map_func(device, gpu_intensity, gpu_label, nullptr, 1, true);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_mode[i], 0.0001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelNeighbortMap, ::testing::ValuesIn(getParameters()));
