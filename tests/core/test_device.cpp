
#include "cle.hpp"

#include "test_utils.hpp"
#include <gtest/gtest.h>

class TestDevice : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
  }
};

TEST_P(TestDevice, availableDevices)
{

  auto devices_all = cle::BackendManager::getInstance().getBackend().getDevices("all");
  auto devices_gpu = cle::BackendManager::getInstance().getBackend().getDevices("gpu");
  auto devices_cpu = cle::BackendManager::getInstance().getBackend().getDevices("cpu");
  if (backend == "cuda" || backend == "metal")
  {
    // GPU-only backends: no device of type "cpu", but asking for one still gives a usable device
    EXPECT_TRUE(devices_cpu.empty());
    if (backend == "cuda")
    {
      EXPECT_NE(cle::BackendManager::getInstance().getBackend().getDevice("", "cpu"), nullptr);
      EXPECT_NE(cle::BackendManager::getInstance().getBackend().getDeviceFromIndex(0, "cpu"), nullptr);
      EXPECT_TRUE(cle::BackendManager::getInstance().getBackend().getDevicesList("cpu").empty());
    }
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


TEST_P(TestDevice, defaultDevice)
{


  auto list = cle::BackendManager::getInstance().getBackend().getDevicesList("all");
  for (auto && i : list)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);
  std::cout << device->getName() << std::endl;
}


TEST_P(TestDevice, type)
{

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Check that the device type is correct
  if (backend == "opencl")
  {
    EXPECT_EQ(device->getType(), cle::Device::Type::OPENCL);
  }
  else if (backend == "cuda")
  {
    EXPECT_EQ(device->getType(), cle::Device::Type::CUDA);
  }
  else if (backend == "metal")
  {
    EXPECT_EQ(device->getType(), cle::Device::Type::METAL);
  }
  else
  {
    FAIL();
  }
}

TEST_P(TestDevice, name)
{

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Check that the device name is not empty
  EXPECT_FALSE(device->getName().empty());
}

TEST_P(TestDevice, index)
{

  auto device = cle::BackendManager::getInstance().getBackend().getDeviceFromIndex(0, "all");
  device->setWaitToFinish(true);

  // Check that the device name is not empty
  EXPECT_FALSE(device->getName().empty());
}

TEST_P(TestDevice, info)
{

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // Check that the device info is not empty
  EXPECT_FALSE(device->getInfo().empty());
}

TEST_P(TestDevice, waitInitFinit)
{
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  // runs a kernel on the device (and caches it)
  const auto add_one = [&device]() {
    auto array = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
    array->fill(2.5F);
    auto               result = cle::tier1::add_image_and_scalar_func(device, array, nullptr, 1);
    std::vector<float> output(4 * 3 * 2);
    result->readTo(output.data());
    for (const float value : output)
    {
      EXPECT_EQ(value, 3.5F);
    }
  };

  device->initialize();
  EXPECT_TRUE(device->isInitialized());
  add_one();

  // finalize() waits for the pending work without changing the wait setting
  device->setWaitToFinish(false);
  device->finalize();
  EXPECT_FALSE(device->isInitialized());
  EXPECT_FALSE(device->getWaitToFinish());

  // a finalized device can be initialized again and used (its kernels are rebuilt for the new context)
  device->initialize();
  ASSERT_TRUE(device->isInitialized());
  device->setWaitToFinish(true);
  add_one();
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
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDevice, ::testing::ValuesIn(getParameters()));
