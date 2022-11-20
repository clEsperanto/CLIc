#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <benchmark_base.cpp>

class DummyBenchmark : public BenchmarkBase
{
protected:
  auto
  Setup() -> void override
  {}

  auto
  Iteration() -> void override
  {
    std::this_thread::sleep_for(std::chrono::microseconds(700));
  };

  auto
  Teardown() -> void override
  {}

  auto
  InterpretTiming(const std::string & title, const unsigned long time) -> void override
  {
    std::cout << title << ": " << static_cast<float>(time - 700) / static_cast<float>(700 * 100) << "% wait inaccuracy"
              << std::endl;
  }

public:
  ~DummyBenchmark() = default;
};

auto
main() -> int
{
  DummyBenchmark dummy;
  dummy.Run();
  return 0;
}
