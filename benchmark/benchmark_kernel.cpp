#include <chrono>
#include <vector>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>

#include "CLE.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;

template<typename T>
T getAverage(const vector<T>& v)
{
        return std::accumulate(v.begin(), v.end(), 0) / v.size();
}

class BenchmarkBase
{
private:
    /// holds the compilation time or -1 if it has not been measured yet
    unsigned int maybeCompilationMs = -1;

    void ExecuteSingleIteration(vector<unsigned long>& timingResultWriteback)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> begin
            = std::chrono::high_resolution_clock::now();

        Iteration();

        std::chrono::time_point<std::chrono::high_resolution_clock> end
            = std::chrono::high_resolution_clock::now();

        timingResultWriteback.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
    }

protected:
    /// called first once
    virtual void Setup() = 0;
    /// called multiple times for warmup & actual testing, the only section that is measured
    virtual void Iteration() = 0;
    /// called once
    virtual void Teardown() = 0;
    /// optional callback to interpret (= provide additional info) the timing results of a single run/avg; ususally prints to stdout. Timings are on stdout anyways
    virtual void InterpretTiming(const string& title, const unsigned long AvgTimeMs) {}

    /// optional callback to measure kernel compile time; only called on demand
    virtual void Compile(cle::CLE& cle) {} 

public:
    // Note: c++ virtual classes need a destructor, so child classes' constructors are called too
    virtual ~BenchmarkBase(){};
    unsigned iterationWarmupCount = 8;
    unsigned iterationNormalCount = 16;
    unsigned iterationCompilationCount = 8;

    vector<unsigned long> iterationNormalTimingsMs;
    vector<unsigned long> iterationWarmupTimingsMs;

    unsigned int GetAvgWarmupMs() const
    {
        return getAverage<unsigned long>(iterationWarmupTimingsMs);
    }

    unsigned int GetAvgNormalMs() const
    {
        return getAverage<unsigned long>(iterationNormalTimingsMs);
    }

    unsigned int GetAvgTotalMs() const
    {
        size_t warmups = iterationWarmupTimingsMs.size();
        size_t normals = iterationNormalTimingsMs.size();
        return (GetAvgWarmupMs() * warmups + GetAvgNormalMs() * normals) / (normals + warmups);
    }

    unsigned long GetCompilationMs()
    {
        if (-1 == maybeCompilationMs) {
            vector<unsigned long> compilationTimings;
            for (int i = 0; i < iterationCompilationCount; i++)
            {
                // always reconstruct the cle object, as it caches the compiled kernels
                cle::GPU gpu;
                cle::CLE cle(gpu);

                // now measure the compilation
                std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();
                Compile(cle);
                std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

                compilationTimings.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
            }

            maybeCompilationMs = getAverage<unsigned long>(compilationTimings);
        }

        return maybeCompilationMs;
    }
                    
    void Run(bool printResults = true)
    {
        if (!printResults)
        {
            cout.setstate(std::ios_base::badbit);
        }
        
        cout << "Running setup...";
        cout.flush();
        Setup();
        cout << "OK" << endl;

        cout << "Warming up [" << iterationWarmupCount << "]";
        cout.flush();

        for (int i = 0; i < iterationWarmupCount; i++)
        {
            ExecuteSingleIteration(iterationWarmupTimingsMs);
            cout << ".";
            cout.flush();
        }
        cout << "OK" << endl;

        cout << "Executing [" << iterationNormalCount << "]";
        cout.flush();
        for (int i = 0; i < iterationNormalCount; i++)
        {
            ExecuteSingleIteration(iterationNormalTimingsMs);
            cout << ".";
            cout.flush();
        }
        cout << "OK" << endl;

        cout << "Tearing down...";
        cout.flush();
        Teardown();
        cout << "OK" << endl;

        InterpretTiming("warmup", GetAvgWarmupMs());
        InterpretTiming("normal", GetAvgNormalMs());
        InterpretTiming("total", GetAvgTotalMs());

        cout.width(8);
        cout << endl;
        cout << endl;
        cout << "Avg Warmup: " << GetAvgWarmupMs() << "ms" << endl;
        cout << "Avg Normal: " << GetAvgNormalMs() << "ms" << endl;
        cout << "Avg Total:  " << GetAvgTotalMs()  << "ms" << endl;
    }
};
