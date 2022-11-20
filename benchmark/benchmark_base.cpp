#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

#include "clesperanto.hpp"

template <typename T>
auto
getAverage(const std::vector<T> & values) -> T
{
  return static_cast<T>(std::accumulate(values.begin(), values.end(), 0) / values.size());
}

class BenchmarkBase
{
private:
  /// holds the compilation time or -1 if it has not been measured yet
  unsigned int maybeCompilationMs = -1;

  void
  ExecuteSingleIteration(std::vector<unsigned long> & timingResultWriteback)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();
    Iteration();
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

    timingResultWriteback.push_back(
      (unsigned long)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
  }

protected:
  /// called first once
  virtual auto
  Setup() -> void = 0;
  /// called multiple times for warmup & actual testing, the only section that is measured
  virtual auto
  Iteration() -> void = 0;
  /// called once
  virtual auto
  Teardown() -> void = 0;
  /// optional callback to interpret (= provide additional info) the timing results of a single run/avg; ususally prints
  /// to stdout. Timings are on stdout anyways
  virtual auto
  InterpretTiming(const std::string & title, const unsigned long AvgTimeMs) -> void
  {}

  /// optional callback to measure kernel compile time; only called on demand; should contain kernel call with minimal
  /// parameters
  virtual auto
  Compile(cle::Clesperanto & cle) -> void
  {}

public:
  // Note: c++ virtual classes need a destructor, so child classes' constructors are called too
  ~BenchmarkBase() = default;
  unsigned iterationWarmupCount = 8;
  unsigned iterationNormalCount = 16;
  unsigned iterationCompilationCount = 8;

  std::vector<unsigned long> iterationNormalTimingsMs;
  std::vector<unsigned long> iterationWarmupTimingsMs;

  /// avg iteration time during warmup runs
  [[nodiscard]] auto
  GetAvgWarmupMs() const -> unsigned int
  {
    return getAverage<unsigned long>(iterationWarmupTimingsMs);
  }

  /// avg iteration time during normal runs
  [[nodiscard]] auto
  GetAvgNormalMs() const -> unsigned int
  {
    return getAverage<unsigned long>(iterationNormalTimingsMs);
  }

  /// avg iteration time during all runs (includes also warmup)
  [[nodiscard]] auto
  GetAvgTotalMs() const -> unsigned int
  {
    size_t warmups = iterationWarmupTimingsMs.size();
    size_t normals = iterationNormalTimingsMs.size();
    return static_cast<unsigned int>((GetAvgWarmupMs() * warmups + GetAvgNormalMs() * normals) / (normals + warmups));
  }

  /// wrapper for the compile function with new environment (which causes kernel recompilation)
  auto
  GetCompilationMs() -> unsigned long
  {
    if (-1 == maybeCompilationMs)
    {
      std::vector<unsigned long> compilationTimings;
      for (unsigned int i = 0; i < iterationCompilationCount; i++)
      {
        // always reconstruct the cle object, as it caches the compiled kernels
        cle::Clesperanto cle;

        // now measure the compilation
        std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();
        Compile(cle);
        std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

        compilationTimings.push_back(
          (unsigned long)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
      }

      maybeCompilationMs = getAverage<unsigned long>(compilationTimings);
    }

    return maybeCompilationMs;
  }

  auto
  Run(bool printResults = true) -> void
  {
    if (!printResults)
    {
      std::cout.setstate(std::ios_base::badbit);
    }

    std::cout << "Running setup...";
    std::cout.flush();
    Setup();
    std::cout << "OK" << std::endl;

    std::cout << "Warming up [" << iterationWarmupCount << "]";
    std::cout.flush();

    for (unsigned int i = 0; i < iterationWarmupCount; i++)
    {
      ExecuteSingleIteration(iterationWarmupTimingsMs);
      std::cout << ".";
      std::cout.flush();
    }
    std::cout << "OK" << std::endl;

    std::cout << "Executing [" << iterationNormalCount << "]";
    std::cout.flush();
    for (unsigned int i = 0; i < iterationNormalCount; i++)
    {
      ExecuteSingleIteration(iterationNormalTimingsMs);
      std::cout << ".";
      std::cout.flush();
    }
    std::cout << "OK" << std::endl;

    std::cout << "Tearing down...";
    std::cout.flush();
    Teardown();
    std::cout << "OK" << std::endl;

    InterpretTiming("warmup", GetAvgWarmupMs());
    InterpretTiming("normal", GetAvgNormalMs());
    InterpretTiming("total", GetAvgTotalMs());

    std::cout.width(8);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Avg Warmup: " << GetAvgWarmupMs() << " microseconds" << std::endl; // " \u03BCs" << std::endl;
    std::cout << "Avg Normal: " << GetAvgNormalMs() << " microseconds" << std::endl; // " \u03BCs" << std::endl;
    std::cout << "Avg Total:  " << GetAvgTotalMs() << " microseconds" << std::endl;  // " \u03BCs" << std::endl;
  }
};
