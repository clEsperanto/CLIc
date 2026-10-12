// Backend micro-benchmark: host overhead per kernel launch, copies, fill and host transfers.
// Compile the same source against two CLIc builds to compare them (see BACKEND_REVIEW_PLAN.md, "Benchmark").
// Uses only the public API. Each measurement does a warm-up run (kernel compilation excluded), ends with a read
// (so asynchronous work is included) and reports the median of 5 runs.
//
// Usage: benchmark_backend_overhead <opencl|metal|cuda> <wait:0|1>
#include "cle.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <functional>
#include <vector>

static double
median_ms(const std::function<void()> & run, int repeats = 5)
{
  run(); // warm-up (kernel compilation, allocations)
  std::vector<double> t;
  for (int i = 0; i < repeats; ++i)
  {
    auto start = std::chrono::steady_clock::now();
    run();
    t.push_back(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count());
  }
  std::sort(t.begin(), t.end());
  return t[t.size() / 2];
}

int
main(int argc, char ** argv)
{
  if (argc < 3)
  {
    std::printf("Usage: %s <opencl|metal|cuda> <wait:0|1>\n", argv[0]);
    return 1;
  }
  const std::string backend = argv[1];
  const bool        wait = std::atoi(argv[2]) != 0;
  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(wait);
  float sink = 0;

  auto small_a = cle::Array::create(64, 64, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto small_b = cle::Array::create(small_a);
  small_a->fill(0);
  const double t_small = median_ms([&] {
    for (int i = 0; i < 1000; ++i)
    {
      cle::tier1::add_image_and_scalar_func(device, (i % 2) ? small_b : small_a, (i % 2) ? small_a : small_b, 1);
    }
    small_a->readTo(&sink, 0, 0, 0);
  });

  auto big_a = cle::Array::create(1024, 1024, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto big_b = cle::Array::create(big_a);
  big_a->fill(0);
  const double t_big = median_ms([&] {
    for (int i = 0; i < 200; ++i)
    {
      cle::tier1::add_image_and_scalar_func(device, (i % 2) ? big_b : big_a, (i % 2) ? big_a : big_b, 1);
    }
    big_a->readTo(&sink, 0, 0, 0);
  });

  auto vol = cle::Array::create(256, 256, 32, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  vol->fill(1);
  auto         vol_out = cle::Array::create(vol);
  const double t_gauss = median_ms([&] {
    for (int i = 0; i < 20; ++i)
    {
      cle::tier1::gaussian_blur_func(device, vol, vol_out, 2, 2, 1);
    }
    vol_out->readTo(&sink, 0, 0, 0);
  });

  auto         copy_dst = cle::Array::create(big_a);
  const double t_copy = median_ms([&] {
    for (int i = 0; i < 200; ++i)
    {
      big_a->copyTo(copy_dst);
    }
    copy_dst->readTo(&sink, 0, 0, 0);
  });

  auto         fill_arr = cle::Array::create(512, 512, 16, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  const double t_fill = median_ms([&] {
    for (int i = 0; i < 50; ++i)
    {
      fill_arr->fill(static_cast<float>(i));
    }
    fill_arr->readTo(&sink, 0, 0, 0);
  });

  std::vector<float> host(1024 * 1024);
  const double       t_rw = median_ms([&] {
    for (int i = 0; i < 50; ++i)
    {
      big_a->writeFrom(host.data());
      big_a->readTo(host.data());
    }
  });

  std::printf("%-7s wait=%d | 1000x add 64x64: %8.2f ms | 200x add 1024^2: %8.2f ms | 20x gauss 256^2x32: %8.2f ms | "
              "200x copy 1024^2: %8.2f ms | 50x fill 512^2x16: %8.2f ms | 50x write+read 1024^2: %8.2f ms\n",
              backend.c_str(),
              wait,
              t_small,
              t_big,
              t_gauss,
              t_copy,
              t_fill,
              t_rw);
  return 0;
}
