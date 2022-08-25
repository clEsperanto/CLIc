
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "clesperanto.hpp"

#include <benchmark_base.cpp>

class MeanBoxBenchmark : public BenchmarkBase
{
protected:
  cle::Clesperanto cle;
  cle::Image       gpuInput;
  cle::Image       gpuOutput;

  auto
  Setup() -> void override
  {
    std::vector<float> inputData(dataWidth * dataWidth);

    std::array<size_t, 3> dim{ { dataWidth, dataWidth, 1 } };
    gpuInput = cle.Push<float>(inputData, dim);
    gpuOutput = cle.Create<float>(dim);
  }

  auto
  Teardown() -> void override
  {}

public:
  size_t dataWidth = 0;
  int    radius = 3;

  explicit MeanBoxBenchmark(const cle::Clesperanto & _cle)
    : cle(_cle)
  {}

  MeanBoxBenchmark() = default;
  ~MeanBoxBenchmark() = default;
};

class MeanBoxNonSeperableBenchmark : public MeanBoxBenchmark
{
protected:
  auto
  Iteration() -> void override
  {
    cle.MeanSphere(gpuInput, gpuOutput, radius, radius);
  }

  auto
  Compile(cle::Clesperanto & cle) -> void override
  {
    std::array<size_t, 3> dim{ { 1, 1, 1 } };
    cle::Image            in = cle.Create<float>(dim);
    cle::Image            out = cle.Create<float>(dim);
    cle.MeanSphere(in, out, 1, 1);
  }

public:
  explicit MeanBoxNonSeperableBenchmark(const cle::Clesperanto & _cle)
    : MeanBoxBenchmark(_cle)
  {}

  MeanBoxNonSeperableBenchmark() = default;
  ~MeanBoxNonSeperableBenchmark() = default;
};

class MeanBoxSeperableBenchmark : public MeanBoxBenchmark
{
protected:
  auto
  Iteration() -> void override
  {
    cle.MeanBox(gpuInput, gpuOutput, static_cast<float>(radius), static_cast<float>(radius));
  }

  auto
  Compile(cle::Clesperanto & cle) -> void override
  {
    std::array<size_t, 3> dim{ { 1, 1, 1 } };
    cle::Image            in = cle.Create<float>(dim);
    cle::Image            out = cle.Create<float>(dim);
    cle.MeanBox(in, out, 1, 1);
  }

public:
  explicit MeanBoxSeperableBenchmark(const cle::Clesperanto & _cle)
    : MeanBoxBenchmark(_cle)
  {}
  MeanBoxSeperableBenchmark() = default;
  ~MeanBoxSeperableBenchmark() = default;
};

template <class T>
std::map<size_t, unsigned long>
getTimingsBySizes(const size_t maxSize)
{
  static cle::Clesperanto cle;

  std::map<size_t, unsigned long> timings;
  for (size_t elemCnt = 1; elemCnt <= maxSize; elemCnt <<= 1)
  {
    std::cout << "\n\n# Bytes: " << elemCnt * elemCnt * sizeof(float) << "\n###" << std::endl;
    T d(cle);
    d.dataWidth = static_cast<int>(elemCnt);
    d.Run();

    timings[elemCnt] = d.GetAvgNormalMs();
  }

  return timings;
}

template <class T>
std::map<size_t, unsigned long>
getTimingsByRadius(const size_t elemCnt)
{
  static cle::Clesperanto cle;

  std::map<size_t, unsigned long> timings;

  for (size_t radius = 1; radius <= 32; radius++)
  {
    std::cout << "\n\n# Radius " << radius << "\n###" << std::endl;
    T d(cle);
    d.dataWidth = static_cast<int>(elemCnt);
    d.radius = static_cast<int>(radius);
    d.Run();

    timings[radius] = d.GetAvgNormalMs();
  }

  return timings;
}

static void
writeTimings(std::basic_ostream<char> & s, const unsigned long compileMs, const unsigned long runMs)
{
  s << compileMs << "\t";
  s << runMs << "\t";
  s << compileMs + runMs;
}

auto
main(int argc, char ** argv) -> int
{
  if (argc < 3)
  {
    std::cout << "usage: " << argv[0] << " FILE FILE [SIZE]" << std::endl;
    return 1;
  }

  std::ofstream timingsSize(argv[1], std::ios_base::trunc | std::ios_base::out);
  if (!timingsSize.is_open())
  {
    std::cout << "could not open for writing: " << argv[1] << std::endl;
    return 1;
  }

  std::ofstream timingsRadius(argv[2], std::ios_base::trunc | std::ios_base::out);
  if (!timingsRadius.is_open())
  {
    std::cout << "could not open for writing: " << argv[2] << std::endl;
    return 1;
  }

  size_t maxSize = 1 << 13;
  if (argc >= 4)
  {
    maxSize = 1ULL << atoi(argv[3]);
    std::cout << "max square width: " << maxSize << std::endl;
  }

  std::cout << "Seperable (former: 2D Box)" << std::endl;
  auto timingsSeperableMsBySize = getTimingsBySizes<MeanBoxSeperableBenchmark>(maxSize);
  std::cout << "Non-Seperable (former: 2D Sphere)" << std::endl;
  auto timingsNonSeperableMsBySize = getTimingsBySizes<MeanBoxNonSeperableBenchmark>(maxSize);
  std::cout << std::endl << std::endl;

  std::cout << "Measuring Kernel compile Times...";
  std::cout.flush();
  MeanBoxSeperableBenchmark benchSep;
  benchSep.iterationCompilationCount = 32;
  unsigned int                 compileSeperableMs = benchSep.GetCompilationMs();
  MeanBoxNonSeperableBenchmark benchNonSep;
  benchNonSep.iterationCompilationCount = 32;
  unsigned int compileNonSeperableMs = benchNonSep.GetCompilationMs();
  std::cout << "OK" << std::endl;

  std::cout << "saving results...";
  std::cout.flush();

  timingsSize << "width\telem_cnt\tradius\tsep_compile_ms\tsep_run_ms\tsep_total_ms\tnonsep_compile_ms\tnonsep_run_"
                 "ms\tnonsep_total_ms\n";
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
  std::cout << "OK" << std::endl;

  const size_t defaultDataWidth = maxSize;
  std::cout << std::endl << std::endl;
  std::cout << "Measurements for different radiuses" << std::endl;
  std::cout << "Bytes: " << defaultDataWidth * defaultDataWidth * sizeof(float) << std::endl;
  std::cout << std::endl;

  std::cout << "Seperable (former: 2D Box)" << std::endl;
  auto timingsSeperableMsByRadius = getTimingsByRadius<MeanBoxSeperableBenchmark>(defaultDataWidth);
  std::cout << "Non-Seperable (former: 2D Sphere)" << std::endl;
  auto timingsNonSeperableMsByRadius = getTimingsByRadius<MeanBoxNonSeperableBenchmark>(defaultDataWidth);

  std::cout << "saving results...";
  std::cout.flush();

  timingsRadius << "width\telem_cnt\tradius\tsep_compile_ms\tsep_run_ms\tsep_total_ms\tnonsep_compile_ms\tnonsep_run_"
                   "ms\tnonsep_total_ms\n";
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
  std::cout << "OK" << std::endl;

  return 0;
}
