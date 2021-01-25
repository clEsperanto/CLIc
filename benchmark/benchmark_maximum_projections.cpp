#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <benchmark_kernel.cpp>

#include "CLE.h"

#define WIDTH 512
#define HEIGHT 512
#define DEPTH 512

using std::string;
using std::cout;

class MaximumProjectionBenchmarkBase : public BenchmarkBase
{
protected:
    cle::GPU gpu;
    cle::CLE cle;
    cle::Buffer gpuInput, gpuOutput;
    float* input_data;

    virtual void Setup()
    {
        input_data = new float[WIDTH * HEIGHT * DEPTH];
        std::default_random_engine generator;
        std::normal_distribution<float> distribution(5.0,2.0);
        for (size_t d = 0; d < DEPTH; d++)
        {
            for (size_t y = 0; y < HEIGHT; y++)
            {
                for (size_t x = 0; x < WIDTH; x++)
                {
                    int i = x + WIDTH*(y+HEIGHT*d);
                    if ( x == y )
                    {
                        input_data[i] = 1000;
                    }
                    else
                    {
                        input_data[i] = distribution(generator);
                    }
                }
            }
        }
        Image<float> input_img (input_data, WIDTH, HEIGHT, DEPTH, "float");

        // Initialise GPU information.
        cle::GPU gpu;
        cle::CLE cle(gpu);

        // Initialise device memory and push from host
        std::array<unsigned int, 3> dimensions = {WIDTH, HEIGHT, DEPTH};
        dimensions.back() = 1;
        gpuInput = cle.Push<float>(input_img);
        gpuOutput = cle.Create<float>(dimensions.data(), "float");
    };

    virtual void Iteration() {};

    virtual void Teardown()
    {
        delete[] input_data;
    };

    /* virtual void InterpretTiming(const string& title, const unsigned long ms) { */
    /*     cout << title << ": " <<  ((float)(ms - 700)/(float)700 * 100) << "% wait inaccuracy" << endl; */
    /* }; */

public:
    MaximumProjectionBenchmarkBase() : gpu(), cle(cle::CLE(gpu)) {}
    virtual ~MaximumProjectionBenchmarkBase(){}
};

class MaximumXProjectionBenchmark : public MaximumProjectionBenchmarkBase
{
protected:
    virtual void Iteration()
    {
        cle.MaximumXProjection(gpuInput, gpuOutput);
    };

public:
    MaximumXProjectionBenchmark() {}
    virtual ~MaximumXProjectionBenchmark(){}
};

class MaximumYProjectionBenchmark : public MaximumProjectionBenchmarkBase
{
protected:
    virtual void Iteration()
    {
        cle.MaximumYProjection(gpuInput, gpuOutput);
    };

public:
    MaximumYProjectionBenchmark() {}
    virtual ~MaximumYProjectionBenchmark(){}
};

class MaximumZProjectionBenchmark : public MaximumProjectionBenchmarkBase
{
protected:
    virtual void Iteration()
    {
        cle.MaximumZProjection(gpuInput, gpuOutput);
    };

public:
    MaximumZProjectionBenchmark() {}
    virtual ~MaximumZProjectionBenchmark(){}
};

int main() {
    std::cout << "MaximumXProjection: " << std::endl;
    MaximumXProjectionBenchmark x;
    x.Run();
    std::cout << "MaximumYProjection: " << std::endl;
    MaximumYProjectionBenchmark y;
    y.Run();
    std::cout << "MaximumZProjection: " << std::endl;
    MaximumZProjectionBenchmark z;
    z.Run();
    return 0;
}
