
#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    for (auto it = input.begin (), it_valid = valid.begin ();
         (it != input.end ()) && (it_valid != valid.end ()); ++it, ++it_valid)
        {
            *it = static_cast<type> ((int)rand () % 10);
            *it_valid = *it;
            if ((it - input.begin ()) % shape[0] == 1)
                {
                    *it_valid = static_cast<type> (100);
                }
        }

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    cle.SetColumn (gpu_input, 1, 100);
    auto output = cle.Pull<type> (gpu_input);

    std::copy (std::begin (input),
               std::end (input),
               std::ostream_iterator<type> (std::cout, ", "));
    std::cout << std::endl;

    std::copy (std::begin (valid),
               std::end (valid),
               std::ostream_iterator<type> (std::cout, ", "));
    std::cout << std::endl;

    std::copy (std::begin (output),
               std::end (output),
               std::ostream_iterator<type> (std::cout, ", "));
    std::cout << std::endl;

    return std::equal (output.begin (), output.end (), valid.begin ());
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
    assert (run_test<float> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 10, 7, 5 }, CL_MEM_OBJECT_BUFFER));
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
    assert (run_test<float> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}