
#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    input = { 0, 0, 1, 1, 0, 0,
              0, 1, 8, 9, 1, 0,
              0, 1, 7, 6, 1, 0,
              0, 0, 1, 1, 1, 0,
              0, 0, 1, 1, 1, 0,
              0, 0, 1, 8, 7, 1,
              0, 0, 1, 1, 1, 0 };
    valid = { 0, 0, 1, 1, 0, 0,
              0, 1, 1, 1, 1, 0,
              0, 1, 1, 1, 1, 0,
              0, 0, 2, 2, 2, 0,
              0, 0, 0, 2, 2, 0,
              0, 0, 2, 2, 2, 0,
              0, 0, 0, 2, 2, 0 };

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    auto gpu_output = cle.Create<type> ({ shape[0], shape[1], 1 }, mem_type);
    cle.VoronoiOtsuLabeling (gpu_input, gpu_output, 1, 1);
    auto output = cle.Pull<type> (gpu_output);

    return std::equal (output.begin (), output.end (), valid.begin ());
}

auto
main (int argc, char **argv) -> int
{
    // assert (run_test<float> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<int> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned int> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<short> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned short> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<char> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned char> ({ 10, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<float> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 6, 7, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<float> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<int> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned int> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<short> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned short> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<char> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned char> ({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 6, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}