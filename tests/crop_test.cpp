
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
    //std::vector<type> input(shape[0] * shape[1] * shape[2]);
    //std::vector<type> valid(shape[0] * shape[1] * shape[2]);

    std::vector<type> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::vector<type> valid{6, 7, 10, 11};

    const int index0 = 1;
    const int index1 = 1;
    const int index2 = 0;

    //std::fill(input.begin(), input.end(), static_cast<type>(value));
    //std::fill(valid.begin(), valid.end(), static_cast<type> );

    cle::Clesperanto cle;
    cle.GetDevice()->WaitForKernelToFinish();
    auto gpu_input = cle.Push<type>(input, shape, mem_type);
    auto gpu_output = cle.Create<type>({2, 2, 1}, mem_type);
    cle.Crop(gpu_input, gpu_output, index0, index1, index2);
    auto output = cle.Pull<type>(gpu_output);

    return std::equal(output.begin(), output.end(), valid.begin());
    
}

auto
main(int argc, char ** argv) -> int
{
    if(!run_test<int32_t>({4, 4, 1}, cle::BUFFER))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}