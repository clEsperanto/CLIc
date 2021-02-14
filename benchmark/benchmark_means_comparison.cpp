#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <ostream>

#include "CLE.h"

#include <benchmark_base.cpp>

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
        
        vector<unsigned int> dim{{dataWidth, dataWidth, 1}};
        gpuInput = cle.Push<float>(inputData, dim.data());
        gpuOutput = cle.Create<float>(dim.data());
    }
    virtual void Teardown() {}

public:
    unsigned dataWidth;
    unsigned radius = 3;
    Mean2DBoxBenchmark(const cle::GPU& _gpu, const cle::CLE& _cle) : gpu(_gpu), cle(_cle){}
    Mean2DBoxBenchmark() : gpu(cle::GPU()), cle(cle::CLE(gpu)){}
    virtual ~Mean2DBoxBenchmark(){}
};

class Mean2DBoxNonSeperableBenchmark : public Mean2DBoxBenchmark
{
protected:
    virtual void Iteration()
    {
        cle.Mean2DSphere(gpuInput, gpuOutput, radius, radius);
    }

    virtual void Compile(cle::CLE& cle)
    {
        vector<unsigned int> dim{{1, 1, 1}};
        cle::Buffer in = cle.Create<float>(dim.data());
        cle::Buffer out = cle.Create<float>(dim.data());
        cle.Mean2DSphere(in, out, 1, 1);
    }

public:
    Mean2DBoxNonSeperableBenchmark(const cle::GPU& _gpu, const cle::CLE& _cle) : Mean2DBoxBenchmark(_gpu, _cle){}
    Mean2DBoxNonSeperableBenchmark() : Mean2DBoxBenchmark(){}
    virtual ~Mean2DBoxNonSeperableBenchmark(){}
};

class Mean2DBoxSeperableBenchmark : public Mean2DBoxBenchmark
{
protected:

    virtual void Iteration()
    {
        cle.Mean2DBox(gpuInput, gpuOutput, radius, radius);
    }

    virtual void Compile(cle::CLE& cle)
    {
        vector<unsigned int> dim{{1, 1, 1}};
        cle::Buffer in = cle.Create<float>(dim.data());
        cle::Buffer out = cle.Create<float>(dim.data());
        cle.Mean2DBox(in, out, 1, 1);
    }

public:
    Mean2DBoxSeperableBenchmark(const cle::GPU& _gpu, const cle::CLE& _cle) : Mean2DBoxBenchmark(_gpu, _cle){}
    Mean2DBoxSeperableBenchmark() : Mean2DBoxBenchmark(){}
    virtual ~Mean2DBoxSeperableBenchmark(){}
};

template<class T>
map<size_t, unsigned long> getTimingsBySizes(const size_t maxSize)
{
    static cle::GPU gpu;
    static cle::CLE cle(gpu);

    map<size_t, unsigned long> timings;
    for (size_t elemCnt = 1; elemCnt <= maxSize; elemCnt <<= 1)
    {
        cout << "\n\n# Bytes: " << elemCnt*elemCnt*sizeof(float) << "\n###" << endl;
        T d(gpu, cle);
        d.dataWidth = elemCnt;
        d.Run();

        timings[elemCnt] = d.GetAvgNormalMs();
    }

    return timings;
}

template<class T>
map<size_t, unsigned long> getTimingsByRadius(const size_t elemCnt)
{
    static cle::GPU gpu;
    static cle::CLE cle(gpu);

    map<size_t, unsigned long> timings;

    for (size_t radius = 1; radius <= 32; radius++)
    {
        cout << "\n\n# Radius " << radius << "\n###" << endl;
        T d(gpu, cle);
        d.dataWidth = elemCnt;
        d.radius = radius;
        d.Run();

        timings[radius] = d.GetAvgNormalMs();
    }

    return timings;
}

static void writeTimings(std::basic_ostream<char>& s, const unsigned long compileMs, const unsigned long runMs) {
    s << compileMs << "\t";
    s << runMs << "\t";
    s << compileMs + runMs;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "usage: " << argv[0] << " FILE FILE [SIZE]" << endl;
        return 1;
    }

    std::ofstream timingsSize(argv[1], std::ios_base::trunc | std::ios_base::out);
    if (!timingsSize.is_open()) {
        cout << "could not open for writing: " << argv[1] << endl;
        return 1;
    }

    std::ofstream timingsRadius(argv[2], std::ios_base::trunc | std::ios_base::out);
    if (!timingsRadius.is_open()) {
        cout << "could not open for writing: " << argv[2] << endl;
        return 1;
    }

    size_t maxSize = 1 << 13;
    if (argc >= 4) {
        maxSize = 1 << atoi(argv[3]);
        cout << "max square width: " << maxSize << endl;
    }

    cout << "Seperable (former: 2D Box)" << endl;
    auto timingsSeperableMsBySize = getTimingsBySizes<Mean2DBoxSeperableBenchmark>(maxSize);
    cout << "Non-Seperable (former: 2D Sphere)" << endl;
    auto timingsNonSeperableMsBySize = getTimingsBySizes<Mean2DBoxNonSeperableBenchmark>(maxSize);
    cout << endl << endl;

    cout << "Measuring Kernel compile Times...";
    cout.flush();
    Mean2DBoxSeperableBenchmark benchSep;
    benchSep.iterationCompilationCount = 32;
    unsigned int compileSeperableMs = benchSep.GetCompilationMs();
    Mean2DBoxNonSeperableBenchmark benchNonSep;
    benchNonSep.iterationCompilationCount = 32;
    unsigned int compileNonSeperableMs = benchNonSep.GetCompilationMs();
    cout << "OK" << endl;

    cout << "saving results...";
    cout.flush();
    
    timingsSize << "width\telem_cnt\tradius\tsep_compile_ms\tsep_run_ms\tsep_total_ms\tnonsep_compile_ms\tnonsep_run_ms\tnonsep_total_ms\n";
    for (size_t elemCnt = 1; elemCnt <= maxSize; elemCnt <<= 1)
    {
        timingsSize << elemCnt << "\t";
        timingsSize << elemCnt * elemCnt << "\t";
        // use default radius
        timingsSize << benchSep.radius << "\t";

        writeTimings(timingsSize, compileSeperableMs, timingsSeperableMsBySize[elemCnt]);
        timingsSize << "\t";
            
        writeTimings(timingsSize, compileNonSeperableMs, timingsNonSeperableMsBySize[elemCnt]);
        timingsSize << "\n";
    }
    cout << "OK" << endl;

    const size_t defaultDataWidth = maxSize;
    cout << endl << endl;
    cout << "Measurements for different radiuses" << endl;
    cout << "Bytes: " << defaultDataWidth * defaultDataWidth * sizeof(float) << endl;
    cout << endl;

    cout << "Seperable (former: 2D Box)" << endl;
    auto timingsSeperableMsByRadius = getTimingsByRadius<Mean2DBoxSeperableBenchmark>(defaultDataWidth);
    cout << "Non-Seperable (former: 2D Sphere)" << endl;
    auto timingsNonSeperableMsByRadius = getTimingsByRadius<Mean2DBoxNonSeperableBenchmark>(defaultDataWidth);

    cout << "saving results...";
    cout.flush();
    
    timingsRadius << "width\telem_cnt\tradius\tsep_compile_ms\tsep_run_ms\tsep_total_ms\tnonsep_compile_ms\tnonsep_run_ms\tnonsep_total_ms\n";
    for (size_t radius = 1; radius <= 32; radius++)
    {
        timingsRadius << defaultDataWidth << "\t";
        // use default radius
        timingsRadius << defaultDataWidth * defaultDataWidth << "\t";
        timingsRadius << radius << "\t";

        writeTimings(timingsRadius, compileSeperableMs, timingsSeperableMsByRadius[radius]);
        timingsRadius << "\t";
            
        writeTimings(timingsRadius, compileNonSeperableMs, timingsNonSeperableMsByRadius[radius]);
        timingsRadius << "\n";
    }
    cout << "OK" << endl;

    return 0;
}
