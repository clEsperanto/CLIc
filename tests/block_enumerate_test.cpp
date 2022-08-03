
#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    type index = 0;
    for (auto i = 0; i < valid.size (); i++)
        {
            if (i % 2 == 0)
                {
                    valid[i] = ++index;
                    input[i] = 1;
                }
            else
                {
                    valid[i] = 0;
                    input[i] = 0;
                }
        }

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    int blocksize = 4;
    auto gpu_input1 = cle.Push<type> (input, shape);
    size_t block_value = static_cast<size_t> ((static_cast<size_t> (shape[0] - 1) + 1) / blocksize) + 1;
    auto gpu_input2 = cle.Create<type> ({ block_value, 1, 1 }, mem_type);
    auto gpu_output = cle.Create<type> (shape, mem_type);
    cle.SumReductionX (gpu_input1, gpu_input2, blocksize);
    cle.BlockEnumerate (gpu_input1, gpu_input2, gpu_output, blocksize);
    auto output = cle.Pull<type> (gpu_output);

    return std::equal (output.begin (), output.end (), valid.begin ());
}

auto
main (int argc, char **argv) -> int
{
    assert (run_test<float> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 5, 1, 1 }, CL_MEM_OBJECT_BUFFER));

    // assert (run_test<float> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<int> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned int> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<short> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned short> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<char> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned char> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    // assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));

    // assert (run_test<float> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<int> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned int> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<short> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned short> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<char> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned char> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<float> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<int> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned int> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<short> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned short> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<char> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned char> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    // assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}