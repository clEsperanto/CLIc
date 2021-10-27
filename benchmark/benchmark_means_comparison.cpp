#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <ostream>

#include "clesperanto.hpp"

#include <benchmark_base.cpp>

using std::string;
using std::vector;
using std::array;
using std::cout;
using std::map;

class MeanBoxBenchmark : public BenchmarkBase
{
protected:
    cle::Clesperanto cle;
    cle::Buffer gpuInput, gpuOutput;

    virtual void Setup()
    {
        vector<float> inputData(dataWidth * dataWidth);
        
        array<int,3> dim{{dataWidth, dataWidth, 1}};
        gpuInput = cle.Push<float>(inputData, dim);
        gpuOutput = cle.Create<float>(dim);
    }
    virtual void Teardown() {}

public:
    int dataWidth;
    int radius = 3;
    MeanBoxBenchmark(const cle::Clesperanto& _cle) : cle(_cle){}
    MeanBoxBenchmark() : cle(cle::Clesperanto()){}
    virtual ~MeanBoxBenchmark(){}
};

class MeanBoxNonSeperableBenchmark : public MeanBoxBenchmark
{
protected:
    virtual void Iteration()
    {
        cle.MeanSphere(gpuInput, gpuOutput, radius, radius);
    }

    virtual void Compile(cle::Clesperanto& cle)
    {
        array<int,3> dim{{1, 1, 1}};
        cle::Buffer in = cle.Create<float>(dim);
        cle::Buffer out = cle.Create<float>(dim);
        cle.MeanSphere(in, out, 1, 1);
    }

public:
    MeanBoxNonSeperableBenchmark(const cle::Clesperanto& _cle) : MeanBoxBenchmark(_cle){}
    MeanBoxNonSeperableBenchmark() : MeanBoxBenchmark(){}
    virtual ~MeanBoxNonSeperableBenchmark(){}
};

class MeanBoxSeperableBenchmark : public MeanBoxBenchmark
{
protected:

    virtual void Iteration()
    {
        cle.MeanBox(gpuInput, gpuOutput, radius, radius);
    }

    virtual void Compile(cle::Clesperanto& cle)
    {
        array<int,3> dim{{1, 1, 1}};
        cle::Buffer in = cle.Create<float>(dim);
        cle::Buffer out = cle.Create<float>(dim);
        cle.MeanBox(in, out, 1, 1);
    }

public:
    MeanBoxSeperableBenchmark(const cle::Clesperanto& _cle) : MeanBoxBenchmark(_cle){}
    MeanBoxSeperableBenchmark() : MeanBoxBenchmark(){}
    virtual ~MeanBoxSeperableBenchmark(){}
};

template<class T>
map<size_t, unsigned long> getTimingsBySizes(const size_t maxSize)
{
    static cle::Clesperanto cle;

    map<size_t, unsigned long> timings;
    for (size_t elemCnt = 1; elemCnt <= maxSize; elemCnt <<= 1)
    {
        cout << "\n\n# Bytes: " << elemCnt*elemCnt*sizeof(float) << "\n###" << endl;
        T d(cle);
        d.dataWidth = elemCnt;
        d.Run();

        timings[elemCnt] = d.GetAvgNormalMs();
    }

    return timings;
}

template<class T>
map<size_t, unsigned long> getTimingsByRadius(const size_t elemCnt)
{
    static cle::Clesperanto cle;

    map<size_t, unsigned long> timings;

    for (size_t radius = 1; radius <= 32; radius++)
    {
        cout << "\n\n# Radius " << radius << "\n###" << endl;
        T d(cle);
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
    auto timingsSeperableMsBySize = getTimingsBySizes<MeanBoxSeperableBenchmark>(maxSize);
    cout << "Non-Seperable (former: 2D Sphere)" << endl;
    auto timingsNonSeperableMsBySize = getTimingsBySizes<MeanBoxNonSeperableBenchmark>(maxSize);
    cout << endl << endl;

    cout << "Measuring Kernel compile Times...";
    cout.flush();
    MeanBoxSeperableBenchmark benchSep;
    benchSep.iterationCompilationCount = 32;
    unsigned int compileSeperableMs = benchSep.GetCompilationMs();
    MeanBoxNonSeperableBenchmark benchNonSep;
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
    auto timingsSeperableMsByRadius = getTimingsByRadius<MeanBoxSeperableBenchmark>(defaultDataWidth);
    cout << "Non-Seperable (former: 2D Sphere)" << endl;
    auto timingsNonSeperableMsByRadius = getTimingsByRadius<MeanBoxNonSeperableBenchmark>(defaultDataWidth);

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
