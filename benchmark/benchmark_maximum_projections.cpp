#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "clesperanto.hpp"
#include <benchmark_base.cpp>

class MaximumProjectionBenchmarkBase : public BenchmarkBase
{
protected:
  cle::Clesperanto cle;
  cle::Image       gpuInput;
  cle::Image       gpuOutput;

  auto
  Setup() -> void override
  {
    std::vector<float> inputData(dataWidth * dataWidth * dataWidth);

    // Initialise device memory and push from host
    std::array<size_t, 3> dimensionsInput = { dataWidth, dataWidth, dataWidth };
    gpuInput = cle.Push<float>(inputData, dimensionsInput);
    std::array<size_t, 3> dimensionsOutput = { dataWidth, dataWidth, 1 };
    gpuOutput = cle.Create<float>(dimensionsOutput);
  }

  auto
  Iteration() -> void override{};

  auto
  Teardown() -> void override{};

public:
  size_t dataWidth = 0;

  MaximumProjectionBenchmarkBase()
    : cle(cle::Clesperanto())
  {}

  ~MaximumProjectionBenchmarkBase() = default;
};

class MaximumXProjectionBenchmark : public MaximumProjectionBenchmarkBase
{
protected:
  auto
  Iteration() -> void override
  {
    cle.MaximumXProjection(gpuInput, gpuOutput);
  }

public:
  MaximumXProjectionBenchmark() = default;
  ~MaximumXProjectionBenchmark() = default;
};

class MaximumYProjectionBenchmark : public MaximumProjectionBenchmarkBase
{
protected:
  auto
  Iteration() -> void override
  {
    cle.MaximumYProjection(gpuInput, gpuOutput);
  }

public:
  MaximumYProjectionBenchmark() = default;
  ~MaximumYProjectionBenchmark() = default;
};

class MaximumZProjectionBenchmark : public MaximumProjectionBenchmarkBase
{
protected:
  auto
  Iteration() -> void override
  {
    cle.MaximumZProjection(gpuInput, gpuOutput);
  }

public:
  MaximumZProjectionBenchmark() = default;
  ~MaximumZProjectionBenchmark() = default;
};

auto
main(int argc, char ** argv) -> int
{
  if (argc < 2)
  {
    std::cout << "usage: " << argv[0] << " FILE" << std::endl;
    return 1;
  }

  MaximumXProjectionBenchmark x;
  MaximumYProjectionBenchmark y;
  MaximumZProjectionBenchmark z;

  std::ofstream csv;
  csv.open(argv[1], std::ios_base::trunc);
  if (!csv.is_open())
  {
    std::cerr << "Could not write to: " << argv[1] << std::endl;
    return 1;
  }

  csv << "width,count,x_runtime_ms,y_runtime_ms,z_runtime_ms" << std::endl;

  for (int width = 1; width < 1024; width *= 2)
  {
    csv << width << "," << width * width * width << ",";
    std::cout << "\n============\n dataWidth = " << width << "\n============";

    std::cout << "\nMaximumXProjection: " << std::endl;
    x.dataWidth = width;
    x.Run();
    csv << x.GetAvgNormalMs() << ",";

    std::cout << "\n\nMaximumYProjection: " << std::endl;
    y.dataWidth = width;
    y.Run();
    csv << y.GetAvgNormalMs() << ",";

    std::cout << "\n\nMaximumZProjection: " << std::endl;
    z.dataWidth = width;
    z.Run();
    csv << z.GetAvgNormalMs() << std::endl;
  }

  return 0;
}
