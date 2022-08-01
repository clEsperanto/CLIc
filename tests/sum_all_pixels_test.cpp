
#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::fill (input.begin (), input.end (), 1);

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    auto gpu_output = cle.Create<type> ({ 1, 1, 1 }, mem_type);
    cle.SumOfAllPixels (gpu_input, gpu_output);
    auto output = cle.Pull<type> (gpu_output).front ();

    return output == input.size ();
}

auto
main (int argc, char **argv) -> int
{
    assert (run_test<float> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<float> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 10, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<float> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}