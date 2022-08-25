#include <iostream>
#include <string>
#include <vector>

#include "clesperanto.hpp"

#include <benchmark_base.cpp>

class MeanBenchmark : public BenchmarkBase
{
protected:
  cle::Clesperanto cle;
  cle::Image       gpuInput;
  cle::Image       gpuOutput;

  auto
  Setup() -> void override
  {
    std::vector<float>    inputData(dataWidth * dataWidth);
    std::array<size_t, 3> dim{ { dataWidth, dataWidth, 1 } };

    cle.GetDevice()->WaitForKernelToFinish();

    // Initialise device memory and push from host
    gpuInput = cle.Push<float>(inputData, dim);
    gpuOutput = cle.Create<float>(dim);
  }

  auto
  Iteration() -> void override
  {
    cle.MeanBox(gpuInput, gpuOutput, 4, 4);
  }

  auto
  Teardown() -> void override
  {}

public:
  size_t dataWidth = 0;

  MeanBenchmark()
    : cle(cle::Clesperanto())
  {}

  ~MeanBenchmark() = default;
};

auto
main(int argc, char ** argv) -> int
{
  MeanBenchmark d;
  d.dataWidth = 1 << 10;

  d.iterationWarmupCount = 18;

  if (argc >= 2)
  {
    d.dataWidth = std::stoi(argv[1]);
    std::cout << "using " << d.dataWidth * d.dataWidth * sizeof(float) << " bytes memory" << std::endl;
  }

  d.Run();

  return 0;
}
