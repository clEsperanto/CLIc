
#include "cleUtils.hpp"
#include "clesperanto.hpp"
#include <random>

template <class type>
std::array<size_t, 3>
generate_data (std::vector<type> &arr_1, std::vector<type> &arr_2, std::vector<type> &valid, size_t width, size_t height, size_t depth)
{
    arr_2.resize (10);
    arr_1.resize (width * height * depth);
    valid.resize (width * height * depth);
    type value = 0;
    for (auto it1 = arr_2.begin (); it1 != arr_2.end (); ++it1)
        {
            *it1 = static_cast<type> ((int)rand () % 10 + 10);
        }
    for (auto it1 = arr_1.begin (), it_valid = valid.begin ();
         (it1 != arr_1.end ()) && (it_valid != valid.end ()); ++it1, ++it_valid)
        {
            value = static_cast<type> ((int)rand () % 10);
            *it1 = value;
            *it_valid = static_cast<type> (arr_2[value]);
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
    auto oclArray_B = cle.Push<type> (arr_2, { arr_2.size (), 1, 1 });
    auto ocl_output = cle.Create<type> (shape);
    cle.ReplaceIntensities (oclArray_A, oclArray_B, ocl_output);
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
    auto oclArray_B = cle.Push<type> (arr_2, { arr_2.size (), 1, 1 }, "image");
    auto ocl_output = cle.Create<type> (shape, "image");
    cle.ReplaceIntensities (oclArray_A, oclArray_B, ocl_output);
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
            std::cerr << "ReplaceIntensities kernel 3d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 5, 1))
        {
            std::cerr << "ReplaceIntensities kernel 2d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 1, 1))
        {
            std::cerr << "ReplaceIntensities kernel 1d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    std::cout << "ReplaceIntensities kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}