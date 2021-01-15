#include <string>
#include <vector>
#include <iostream>

#include "CLE.h"

#include <benchmark_kernel.cpp>

using std::string;
using std::vector;
using std::cout;

class MeanBenchmark : public BenchmarkBase
{
protected:
    cle::GPU gpu;
    cle::CLE cle;
    cle::Buffer gpuInput, gpuOutput;

    virtual void Setup()
    {
        vector<float> inputData(dataWidth * dataWidth);
        Image<float> input_img (inputData.data(), dataWidth, dataWidth, 1, "float");
        
        // Initialise device memory and push from host
        gpuInput = cle.Push<float>(input_img);
        gpuOutput = cle.Create<float>(input_img);
    }

    virtual void Iteration()
    {
        cle.Mean2DBox(gpuInput, gpuOutput, 4, 4);
    }

    virtual void Teardown() {}

    virtual void InterpretTiming(const string& title, const unsigned long ms)
    {
    }


public:
    unsigned dataWidth;
    MeanBenchmark() : gpu(), cle(cle::CLE(gpu)) {}
    virtual ~MeanBenchmark(){}
};

int main(int argc, char** argv) {
    MeanBenchmark d;
    d.dataWidth = 1 << 10;

    d.iterationWarmupCount = 18;

    if (argc >= 2) {
        d.dataWidth = std::stoi(argv[1]);
        cout << "using " << d.dataWidth * d.dataWidth * sizeof(float) << " bytes memory" << endl;
    }

    d.Run();
    return 0;
}
