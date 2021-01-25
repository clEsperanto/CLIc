#include <chrono>
#include <vector>
#include <iostream>
#include <numeric>
#include <string>

using std::vector;
using std::string;
using std::cout;
using std::endl;

class BenchmarkBase
{
private:
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

public:
    // Note: c++ virtual classes need a destructor, so child classes' constructors are called too
    virtual ~BenchmarkBase(){};
    unsigned iterationWarmupCount = 8;
    unsigned iterationNormalCount = 16;

    vector<unsigned long> iterationNormalTimingsMs;
    vector<unsigned long> iterationWarmupTimingsMs;

    unsigned int GetAvgWarmupMs()
    {
        return std::accumulate(iterationWarmupTimingsMs.begin(), iterationWarmupTimingsMs.end(), 0) / iterationWarmupTimingsMs.size();
    }

    unsigned int GetAvgNormalMs()
    {
        return std::accumulate(iterationNormalTimingsMs.begin(), iterationNormalTimingsMs.end(), 0) / iterationNormalTimingsMs.size();
    }
    unsigned int GetAvgTotalMs()
    {
        size_t warmups = iterationWarmupTimingsMs.size();
        size_t normals = iterationNormalTimingsMs.size();
        return (GetAvgWarmupMs() * warmups + GetAvgNormalMs() * normals) / (normals + warmups);
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
