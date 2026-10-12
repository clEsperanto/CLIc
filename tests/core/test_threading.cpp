#include "cle.hpp"
#include "test_utils.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <vector>

// Several threads using the same device concurrently (the Python bindings release the GIL during CLIc calls).
// Run under ThreadSanitizer (-fsanitize=thread) to also catch the data races that don't corrupt results.

constexpr size_t NB_THREADS = 8;
constexpr size_t NB_ITERATIONS = 200;
constexpr size_t WIDTH = 32;
constexpr size_t HEIGHT = 32;

class TestThreading : public ::testing::TestWithParam<std::string>
{};

// Runs body(thread_index) on NB_THREADS threads; returns the messages of the exceptions they threw
static auto
runThreads(const std::function<void(size_t)> & body) -> std::vector<std::string>
{
  std::vector<std::string> errors(NB_THREADS);
  std::vector<std::thread> threads;
  threads.reserve(NB_THREADS);
  for (size_t t = 0; t < NB_THREADS; ++t)
  {
    threads.emplace_back([&, t]() {
      try
      {
        body(t);
      }
      catch (const std::exception & e)
      {
        errors[t] = e.what();
      }
    });
  }
  for (auto & thread : threads)
  {
    thread.join();
  }
  std::vector<std::string> thrown;
  for (const auto & error : errors)
  {
    if (!error.empty())
    {
      thrown.push_back(error);
    }
  }
  return thrown;
}

TEST_P(TestThreading, sameKernelDifferentArguments)
{
  // Same kernel, same device, different scalar and buffer arguments in every thread: the backends must not mix up
  // the arguments of concurrent launches (OpenCL shares the cached cl_kernel between threads)
  cle::BackendManager::getInstance().setBackend(GetParam());
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  std::atomic<size_t> wrong{ 0 };
  const auto          errors = runThreads([&](size_t t) {
    std::vector<float> input(WIDTH * HEIGHT, static_cast<float>(t));
    std::vector<float> output(WIDTH * HEIGHT);
    auto               src = cle::Array::create(WIDTH, HEIGHT, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
    src->writeFrom(input.data());
    const float expected = static_cast<float>(t) + 1000.0F * static_cast<float>(t);
    for (size_t i = 0; i < NB_ITERATIONS; ++i)
    {
      auto dst = cle::tier1::add_image_and_scalar_func(device, src, nullptr, 1000.0F * static_cast<float>(t));
      dst->readTo(output.data());
      for (const auto value : output)
      {
        if (value != expected)
        {
          ++wrong;
          break;
        }
      }
    }
  });

  EXPECT_TRUE(errors.empty()) << errors.front();
  EXPECT_EQ(wrong.load(), 0U);
}

TEST_P(TestThreading, differentKernels)
{
  // Different kernels compiled and looked up concurrently: exercises the shared program cache (LRU updates)
  cle::BackendManager::getInstance().setBackend(GetParam());
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  std::vector<float> input(WIDTH * HEIGHT);
  for (size_t i = 0; i < input.size(); ++i)
  {
    input[i] = static_cast<float>(i % 17);
  }

  // Sequential references, one operation per thread
  auto run = [&](size_t t, const cle::Array::Pointer & src) -> cle::Array::Pointer {
    switch (t % 4)
    {
      case 0:
        return cle::tier1::gaussian_blur_func(device, src, nullptr, 1.0F + static_cast<float>(t), 1.0F, 0.0F);
      case 1:
        return cle::tier1::maximum_box_func(device, src, nullptr, 1.0F + static_cast<float>(t), 1.0F, 0.0F);
      case 2:
        return cle::tier1::multiply_image_and_scalar_func(device, src, nullptr, static_cast<float>(t));
      default:
        return cle::tier2::top_hat_func(device, src, nullptr, 1.0F + static_cast<float>(t), 1.0F, 0.0F, "box");
    }
  };
  std::vector<std::vector<float>> references(NB_THREADS, std::vector<float>(WIDTH * HEIGHT));
  for (size_t t = 0; t < NB_THREADS; ++t)
  {
    auto src = cle::Array::create(WIDTH, HEIGHT, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
    src->writeFrom(input.data());
    run(t, src)->readTo(references[t].data());
  }

  std::atomic<size_t> wrong{ 0 };
  const auto          errors = runThreads([&](size_t t) {
    auto src = cle::Array::create(WIDTH, HEIGHT, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
    src->writeFrom(input.data());
    std::vector<float> output(WIDTH * HEIGHT);
    for (size_t i = 0; i < NB_ITERATIONS / 4; ++i)
    {
      run(t, src)->readTo(output.data());
      if (output != references[t])
      {
        ++wrong;
      }
    }
  });

  EXPECT_TRUE(errors.empty()) << errors.front();
  EXPECT_EQ(wrong.load(), 0U);
}

TEST_P(TestThreading, waitToFinishToggledConcurrently)
{
  // One thread toggles the synchronous mode while the others launch kernels and read results back
  cle::BackendManager::getInstance().setBackend(GetParam());
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  std::atomic<size_t> wrong{ 0 };
  const auto          errors = runThreads([&](size_t t) {
    if (t == 0)
    {
      for (size_t i = 0; i < NB_ITERATIONS; ++i)
      {
        device->setWaitToFinish(i % 2 == 0);
      }
      return;
    }
    std::vector<float> input(WIDTH * HEIGHT, static_cast<float>(t));
    std::vector<float> output(WIDTH * HEIGHT);
    auto               src = cle::Array::create(WIDTH, HEIGHT, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
    src->writeFrom(input.data());
    for (size_t i = 0; i < NB_ITERATIONS / 4; ++i)
    {
      cle::tier1::add_image_and_scalar_func(device, src, nullptr, 1.0F)->readTo(output.data());
      if (output[0] != static_cast<float>(t) + 1.0F)
      {
        ++wrong;
      }
    }
  });
  device->setWaitToFinish(true);

  EXPECT_TRUE(errors.empty()) << errors.front();
  EXPECT_EQ(wrong.load(), 0U);
}

TEST_P(TestThreading, concurrentDeviceQueries)
{
  // Concurrent first device queries (lazy device discovery) must return the same device objects
  cle::BackendManager::getInstance().setBackend(GetParam());
  const auto & backend = cle::BackendManager::getInstance().getBackend();

  std::vector<cle::Device::Pointer> devices(NB_THREADS);
  const auto                        errors = runThreads([&](size_t t) { devices[t] = backend.getDevice("", "all"); });

  EXPECT_TRUE(errors.empty()) << errors.front();
  for (const auto & device : devices)
  {
    EXPECT_EQ(device, devices.front());
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestThreading, ::testing::ValuesIn(getParameters()));
