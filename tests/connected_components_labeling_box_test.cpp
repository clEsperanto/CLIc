
// #include <random>

// #include "cleUtils.hpp"
// #include "clesperanto.hpp"

// template <class type>
// std::array<size_t, 3>
// generate_data (std::vector<type> &arr_1,
//                std::vector<type> &valid, size_t width, size_t height, size_t depth)
// {
//     arr_1.resize (width * height * depth);
//     valid.resize (width * height * depth);
//     if (depth == 2)
//         {
//             arr_1 = { 0, 0, 0, 0, 0,
//                       0, 0, 0, 1, 0,
//                       0, 0, 0, 0, 0,
//                       1, 1, 0, 1, 0,
//                       0, 1, 0, 1, 1,
//                       0, 1, 0, 0, 0 };
//             valid = { 0, 0, 0, 0, 0,
//                       0, 0, 0, 1, 0,
//                       0, 0, 0, 0, 0,
//                       2, 2, 0, 1, 0,
//                       0, 2, 0, 1, 1,
//                       0, 2, 0, 0, 0 };
//         }
//     else if (height == 3)
//         {
//             arr_1 = { 0, 1, 0, 0, 1,
//                       0, 0, 1, 0, 1,
//                       0, 0, 0, 0, 1 };
//             valid = { 0, 1, 0, 0, 2,
//                       0, 0, 1, 0, 2,
//                       0, 0, 0, 0, 2 };
//         }
//     else
//         {
//             arr_1 = { 0, 1, 1, 0, 1 };
//             valid = { 0, 1, 1, 0, 2 };
//         }
//     return std::array<size_t, 3>{ width, height, depth };
// }

#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    if (shape[2] == 2)
        {
            input = { 0, 0, 0, 0, 0,
                      0, 0, 0, 1, 0,
                      0, 0, 0, 0, 0,
                      1, 1, 0, 1, 0,
                      0, 1, 0, 1, 1,
                      0, 1, 0, 0, 0 };
            valid = { 0, 0, 0, 0, 0,
                      0, 0, 0, 1, 0,
                      0, 0, 0, 0, 0,
                      2, 2, 0, 1, 0,
                      0, 2, 0, 1, 1,
                      0, 2, 0, 0, 0 };
        }
    else if (shape[1] == 3)
        {
            input = { 0, 1, 0, 0, 1,
                      0, 0, 1, 0, 1,
                      0, 0, 0, 0, 1 };
            valid = { 0, 1, 0, 0, 2,
                      0, 0, 1, 0, 2,
                      0, 0, 0, 0, 2 };
        }
    else
        {
            input = { 0, 1, 1, 0, 1 };
            valid = { 0, 1, 1, 0, 2 };
        }

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    auto gpu_output = cle.Create<type> (shape, mem_type);
    cle.ConnectedComponentLabelingBox (gpu_input, gpu_output);
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
    assert (run_test<float> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 5, 3, 1 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_BUFFER));
    assert (run_test<float> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned int> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned short> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<unsigned char> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}