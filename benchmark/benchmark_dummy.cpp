#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <benchmark_base.cpp>

using std::string;
using std::cout;

class DummyBenchmark : public BenchmarkBase
{
protected:
  virtual void
  Setup()
  {}
  virtual void
  Iteration()
  {
    std::this_thread::sleep_for(std::chrono::microseconds(700));
  };

  virtual void
  Teardown()
  {}

  virtual void
  InterpretTiming(const string & title, const unsigned long ms)
  {
    cout << title << ": " << ((float)(ms - 700) / (float)700 * 100) << "% wait inaccuracy" << endl;
  }


public:
  virtual ~DummyBenchmark() {}
};

int
main()
{
  DummyBenchmark d;
  d.Run();
  return 0;
}
