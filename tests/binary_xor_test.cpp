
#include <random>

#include "cleUtils.hpp"
#include "clesperanto.hpp"

template <class type>
std::array<size_t, 3>
generate_data (std::vector<type> &arr_1, std::vector<type> &arr_2,
               std::vector<type> &valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize (width * height * depth);
    arr_2.resize (width * height * depth);
    valid.resize (width * height * depth);
    auto it_arr1 = arr_1.begin ();
    auto it_arr2 = arr_2.begin ();
    auto it_valid = valid.begin ();
    while (it_arr1 != arr_1.end () && it_arr2 != arr_2.end () && it_valid != valid.end ())
        {
            *it_arr1 = static_cast<type> ((int)rand () % 2);
            *it_arr2 = static_cast<type> ((int)rand () % 2);
            *it_valid = (*it_arr1) != (*it_arr2);
            ++it_arr1;
            ++it_arr2;
            ++it_valid;
        }
    return std::array<size_t, 3>{ width, height, depth };
}

template <class type>
std::vector<type>
run_kernel_with_buffer (std::vector<type> &arr_1, std::vector<type> &arr_2, std::array<size_t, 3> &shape)
{
    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto oclArray_A = cle.Push<type> (arr_1, shape);
    auto oclArray_B = cle.Push<type> (arr_2, shape);
    auto ocl_output = cle.Create<type> (shape);
    cle.BinaryXor (oclArray_A, oclArray_B, ocl_output);
    auto output = cle.Pull<type> (ocl_output);
    return output;
}

template <class type>
std::vector<type>
run_kernel_with_image (std::vector<type> &arr_1, std::vector<type> &arr_2, std::array<size_t, 3> &shape)
{
    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto oclArray_A = cle.Push<type> (arr_1, shape, "image");
    auto oclArray_B = cle.Push<type> (arr_2, shape, "image");
    auto ocl_output = cle.Create<type> (shape, "image");
    cle.BinaryXor (oclArray_A, oclArray_B, ocl_output);
    auto output = cle.Pull<type> (ocl_output);
    return output;
}

template <class type>
bool
test (size_t width, size_t height, size_t depth)
{
    std::vector<type> arr_1, arr_2, valid;
    std::array<size_t, 3> shape = generate_data<type> (arr_1, arr_2, valid, width, height, depth);
    auto output_buffer = run_kernel_with_buffer<type> (arr_1, arr_2, shape);
    if (IsDifferent (output_buffer, valid))
        {
            std::cerr << "kernel (" << width << "," << height << "," << depth << ") using buffer ... FAILED! " << std::endl;
            return true;
        }
    auto output_image = run_kernel_with_image<type> (arr_1, arr_2, shape);
    if (IsDifferent (output_image, valid))
        {
            std::cerr << "kernel (" << width << "," << height << "," << depth << ") using image ... FAILED! " << std::endl;
            return true;
        }
    return false;
}

int
main (int argc, char **argv)
{
    if (test<float> (10, 5, 2))
        {
            std::cerr << "BinaryXor kernel 3d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 5, 1))
        {
            std::cerr << "BinaryXor kernel 2d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 1, 1))
        {
            std::cerr << "BinaryXor kernel 1d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }

    std::cout << "BinaryXor kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}