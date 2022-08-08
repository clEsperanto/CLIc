#include <iostream>
#include <string>
#include <vector>

#include "clesperanto.hpp"

#include <benchmark_base.cpp>

using std::array;
using std::cout;
using std::string;
using std::vector;

class MeanBenchmark : public BenchmarkBase
{
protected:
  cle::Clesperanto cle;
  cle::Image       gpuInput, gpuOutput;

  virtual void
  Setup()
  {
    vector<float>    inputData(dataWidth * dataWidth);
    array<size_t, 3> dim{ { dataWidth, dataWidth, 1 } };

    cle.GetDevice()->WaitForKernelToFinish();

    // Initialise device memory and push from host
    gpuInput = cle.Push<float>(inputData, dim);
    gpuOutput = cle.Create<float>(dim);
  }

  virtual void
  Iteration()
  {
    cle.MeanBox(gpuInput, gpuOutput, 4, 4);
  }

  virtual void
  Teardown()
  {}

public:
  size_t dataWidth;
  MeanBenchmark()
    : cle(cle::Clesperanto())
  {}
  virtual ~MeanBenchmark() {}
};

int
main(int argc, char ** argv)
{
  MeanBenchmark d;
  d.dataWidth = 1 << 10;

  d.iterationWarmupCount = 18;

  if (argc >= 2)
  {
    d.dataWidth = std::stoi(argv[1]);
    cout << "using " << d.dataWidth * d.dataWidth * sizeof(float) << " bytes memory" << endl;
  }

  d.Run();

  return 0;
}
