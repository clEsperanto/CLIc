#include <iostream>
#include <string>
#include <vector>

#include "clesperanto.hpp"

#include <benchmark_base.cpp>

class PushPullBenchmark : public BenchmarkBase
{
protected:
  cle::Clesperanto      cle;
  std::vector<float>    inputData;
  std::array<size_t, 3> dim;
  auto
  Setup() -> void override
  {
    inputData = std::vector<float>(dataWidth * dataWidth * dataWidth);
    dim = std::array<size_t, 3>{ { dataWidth, dataWidth, dataWidth } };

    cle.GetDevice()->WaitForKernelToFinish();
  }

  auto
  Iteration() -> void override
  {
    auto gpuInput = cle.Push<float>(inputData, dim);
    auto output = cle.Pull<float>(gpuInput);
  }

  auto
  Teardown() -> void override
  {}

public:
  size_t dataWidth = 0;

  PushPullBenchmark()
    : cle(cle::Clesperanto())
  {}

  ~PushPullBenchmark() = default;
};

auto
main(int argc, char ** argv) -> int
{
  PushPullBenchmark d;
  d.dataWidth = 1 << 10;

  d.iterationWarmupCount = 5;

  if (argc >= 2)
  {
    d.dataWidth = std::stoi(argv[1]);
    std::cout << "using " << d.dataWidth * d.dataWidth * d.dataWidth * sizeof(float) << " bytes memory" << std::endl;
  }

  d.Run();

  return 0;
}
