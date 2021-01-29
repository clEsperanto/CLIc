#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <ostream>

#include "CLE.h"

#include <benchmark_kernel.cpp>

using std::string;
using std::vector;
using std::cout;
using std::map;

class Mean2DBoxBenchmark : public BenchmarkBase
{
protected:
    cle::GPU gpu;
    cle::CLE cle;
    cle::Buffer gpuInput, gpuOutput;

    virtual void Setup()
    {
        vector<float> inputData(dataWidth * dataWidth);
        
        // Initialise device memory and push from host
        vector<unsigned int> dim{{dataWidth, dataWidth, 1}};
        gpuInput = cle.Push<float>(inputData, dim.data());
        gpuOutput = cle.Create<float>(dim.data());
    }
    virtual void Teardown() {}

public:
    unsigned dataWidth;
    Mean2DBoxBenchmark() : gpu(), cle(cle::CLE(gpu)) {}
    virtual ~Mean2DBoxBenchmark(){}
};

class Mean2DBoxNonSeperableBenchmark : public Mean2DBoxBenchmark
{
protected:
    virtual void Iteration()
    {
        cle.Mean2DBox(gpuInput, gpuOutput, 4, 4);
        vector<float> out = cle.Pull<float>(gpuOutput);

        if (0.2 == out[0]) {
            // do something, so the compiler doesn't optimize this away
            cout << " \b";
        }
    }

public:
    Mean2DBoxNonSeperableBenchmark() : Mean2DBoxBenchmark() {}
    virtual ~Mean2DBoxNonSeperableBenchmark(){}
};

class Mean2DBoxSeperableBenchmark : public Mean2DBoxBenchmark
{
protected:

    virtual void Iteration()
    {
        cle.Mean2DSphere(gpuInput, gpuOutput, 4, 4);
        vector<float> out = cle.Pull<float>(gpuOutput);

        if (0.2 == out[0]) {
            // do something, so the compiler doesn't optimize this away
            cout << " \b";
        }
    }

public:
    Mean2DBoxSeperableBenchmark() : Mean2DBoxBenchmark() {}
    virtual ~Mean2DBoxSeperableBenchmark(){}
};

template<class T>
map<size_t, unsigned long> getTimingsBySizes(const size_t maxSize)
{
    map<size_t, unsigned long> timings;
    for (size_t elemCnt = 1; elemCnt < maxSize; elemCnt <<= 1)
    {
        cout << "\n\n# Bytes: " << elemCnt*elemCnt*sizeof(float) << "\n###" << endl;
        T d;
        d.iterationWarmupCount = 2;
        d.iterationNormalCount = 6;
        d.dataWidth = elemCnt;
        d.Run();

        timings[elemCnt] = d.GetAvgNormalMs();
    }

    return timings;
}

static void writeTimings(std::basic_ostream<char>& s, const unsigned long compileMs, const unsigned long totalMs) {
    unsigned long runtime = totalMs - compileMs;
    if (compileMs > totalMs) {
        runtime = 0;
    }

    s << compileMs << "\t";
    s << runtime << "\t";
    s << totalMs;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "usage: " << argv[0] << " FILE [SIZE]" << endl;
        return 1;
    }

    std::ofstream timings(argv[1], std::ios_base::trunc | std::ios_base::out);
    if (!timings.is_open()) {
        cout << "could not open for writing: " << argv[1] << endl;
        return 1;
    }

    size_t maxSize = 1 << 13;
    if (argc >= 3) {
        maxSize = 1 << atoi(argv[2]);
        cout << "max square width: " << maxSize << endl;
    }

    cout << "Seperable (former: 2D Box)" << endl;
    auto timingsSeperableMsBySize = getTimingsBySizes<Mean2DBoxSeperableBenchmark>(maxSize);
    cout << "Non-Seperable (former: 2D Sphere)" << endl;
    auto timingsNonSeperableMsBySize = getTimingsBySizes<Mean2DBoxNonSeperableBenchmark>(maxSize);

    
    cout << endl << endl;
    cout << "saving results...";
    cout.flush();
    
    timings << "width\telem_cnt\tsep_compile_ms\tsep_run_ms\tsep_total_ms\tnonsep_compile_ms\tnonsep_run_ms\tnonsep_total_ms\n";
    for (size_t elemCnt = 1; elemCnt < maxSize; elemCnt <<= 1)
    {
        timings << elemCnt << "\t";
        timings << elemCnt * elemCnt << "\t";
            
        writeTimings(timings, timingsSeperableMsBySize[1], timingsSeperableMsBySize[elemCnt]);
        timings << "\t";
            
        writeTimings(timings, timingsNonSeperableMsBySize[1], timingsNonSeperableMsBySize[elemCnt]);
        timings << "\n";
    }

    cout << "OK" << endl;

    return 0;
}
