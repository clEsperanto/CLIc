#include <thread>
#include <chrono>
#include <string>
#include <iostream>

#include <benchmark_kernel.cpp>

using std::string;
using std::cout;

class DummyBenchmark : public BenchmarkBase
{
protected:
    virtual void Setup() {}
    virtual void Iteration()
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(700ms);
    };

    virtual void Teardown() {}

    virtual void InterpretTiming(const string& title, const unsigned long ms) {
        cout << title << ": " <<  ((float)(ms - 700)/(float)700 * 100) << "% wait inaccuracy" << endl;
    }


public:
    virtual ~DummyBenchmark(){}
};

int main() {
    DummyBenchmark d;
    d.Run();
    return 0;
}
