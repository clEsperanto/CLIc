
#include "cle.hpp"

#include <gtest/gtest.h>

class TestDevice : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestDevice, availableDevices)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto devices_all = cle::BackendManager::getInstance().getBackend().getDevices("all");
  auto devices_gpu = cle::BackendManager::getInstance().getBackend().getDevices("gpu");
  auto devices_cpu = cle::BackendManager::getInstance().getBackend().getDevices("cpu");
  if (param == "cuda")
  {
    devices_cpu.clear();
  }

  EXPECT_FALSE(devices_all.empty());
  EXPECT_GE(devices_all.size(), devices_cpu.size());
  EXPECT_GE(devices_all.size(), devices_gpu.size());
  EXPECT_EQ(devices_all.size(), devices_gpu.size() + devices_cpu.size());

  auto devices_wrong = cle::BackendManager::getInstance().getBackend().getDevices("gfds");
  EXPECT_EQ(devices_all.size(), devices_wrong.size());

  auto devices_list_all = cle::BackendManager::getInstance().getBackend().getDevicesList("all");
  EXPECT_FALSE(devices_list_all.empty());
  EXPECT_EQ(devices_all.size(), devices_list_all.size());
}


TEST_P(TestDevice, type)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Check that the device type is correct
  if (param == "opencl")
  {
    EXPECT_EQ(device->getType(), cle::Device::Type::OPENCL);
  }
  else if (param == "cuda")
  {
    EXPECT_EQ(device->getType(), cle::Device::Type::CUDA);
  }
  else
  {
    FAIL();
  }
}

TEST_P(TestDevice, name)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Check that the device name is not empty
  EXPECT_FALSE(device->getName().empty());
}

TEST_P(TestDevice, index)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDeviceFromIndex(0, "all");
  device->setWaitToFinish(true);

  // Check that the device name is not empty
  EXPECT_FALSE(device->getName().empty());
}

TEST_P(TestDevice, info)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Check that the device info is not empty
  EXPECT_FALSE(device->getInfo().empty());
}

TEST_P(TestDevice, waitInitFinit)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Initialize the device
  device->initialize();

  // Check that the device is initialized
  EXPECT_TRUE(device->isInitialized());

  // Finalize the device
  device->finalize();

  // Check that the device is not initialized
  EXPECT_FALSE(device->isInitialized());

  // Set wait to finish to true
  device->setWaitToFinish(true);

  // Set wait to finish to false
  device->setWaitToFinish(false);
}

#if USE_OPENCL
TEST(TestDevice, oclGetters)
{
  cle::BackendManager::getInstance().setBackend("opencl");
  auto generic_device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // cast to OpenCL device
  auto device = dynamic_cast<cle::OpenCLDevice *>(generic_device.get());

  // Get the context
  auto context = device->getCLContext();

  // Check that the context is not null
  EXPECT_NE(context, nullptr);

  // Get the queue
  auto queue = device->getCLCommandQueue();

  // Check that the queue is not null
  EXPECT_NE(queue, nullptr);

  // Get the device
  auto cl_device = device->getCLDevice();

  // Check that the device is not null
  EXPECT_NE(cl_device, nullptr);

  // Get the platform
  auto platform = device->getCLPlatform();

  // Check that the platform is not null
  EXPECT_NE(platform, nullptr);

  // Check that the device type is correct
  EXPECT_EQ(device->getType(), cle::Device::Type::OPENCL);
}
#endif

#if USE_CUDA
TEST(TestDevice, cudaGetters)
{
  cle::BackendManager::getInstance().setBackend("cuda");
  auto generic_device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  // cast to CUDA device
  auto device = dynamic_cast<cle::CUDADevice *>(generic_device.get());

  // Get the context
  auto context = device->getCUDAContext();

  // Check that the context is not null
  EXPECT_NE(context, nullptr);

  // Get the queue
  auto queue = device->getCUDAStream();

  // Check that the queue is not null
  EXPECT_NE(queue, nullptr);

  // Get the device
  auto cu_device = device->getCUDADevice();

  // Check that the device index is >= 0
  EXPECT_GE(cu_device, 0);

  // Get the device index
  auto cu_device_idx = device->getCUDADeviceIndex();

  // Check that the device index is >= 0
  EXPECT_GE(cu_device_idx, 0);

  // Get the device architecture
  auto cu_device_arch = device->getArch();

  // Check that the device architecture is not empty
  EXPECT_FALSE(cu_device_arch.empty());

  // Check that the device type is correct
  EXPECT_EQ(device->getType(), cle::Device::Type::CUDA);
}
#endif

TEST_P(TestDevice, stringCout)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Capture the output of the << operator
  testing::internal::CaptureStdout();
  std::cout << device;
  std::string output = testing::internal::GetCapturedStdout();

  // Check that the output is correct
  std::stringstream expected_output;
  expected_output << "(" << device->getType() << ") " << device->getName();
  EXPECT_EQ(output, expected_output.str());
}

TEST_P(TestDevice, clInfo)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Capture the output of the << operator
  auto info = device->getInfo();
  std::cout << info << std::endl;

  // Capture the output of the << operator
  info = device->getInfoExtended();
  std::cout << info << std::endl;

  // // Check that the output is correct
  // EXPECT_FALSE(output.empty());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDevice, ::testing::ValuesIn(getParameters()));
