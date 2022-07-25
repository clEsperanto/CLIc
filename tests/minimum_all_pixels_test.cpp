
#include "cleUtils.hpp"
#include "clesperanto.hpp"
#include <random>

template <class type>
std::array<size_t, 3>
generate_data (std::vector<type> &arr_1,
               std::vector<type> &valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize (width * height * depth);
    valid.resize (1);
    for (auto it1 = arr_1.begin (); it1 != arr_1.end (); ++it1)
        {
            *it1 = static_cast<type> ((int)rand () % 100 + 10);
        }
    valid[0] = static_cast<type> (1);
    arr_1[(width / 2) + (height / 2) * width + (depth / 2) * height * width] = static_cast<type> (1);
    return std::array<size_t, 3>{ width, height, depth };
}

template <class type>
std::vector<type>
run_kernel_with_buffer (std::vector<type> &arr_1, std::array<size_t, 3> &shape)
{
    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto oclArray_A = cle.Push<type> (arr_1, shape);
    auto ocl_output = cle.Create<type> ();
    cle.MinimumOfAllPixels (oclArray_A, ocl_output);
    auto output = cle.Pull<type> (ocl_output);
    return output;
}

template <class type>
std::vector<type>
run_kernel_with_image (std::vector<type> &arr_1, std::array<size_t, 3> &shape)
{
    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto oclArray_A = cle.Push<type> (arr_1, shape, "image");
    auto ocl_output = cle.Create<type> ({ 1, 1, 1 }, "image");
    cle.MinimumOfAllPixels (oclArray_A, ocl_output);
    auto output = cle.Pull<type> (ocl_output);
    return output;
}

template <class type>
bool
test (size_t width, size_t height, size_t depth)
{
    std::vector<type> arr_1, valid;
    std::array<size_t, 3> shape = generate_data<type> (arr_1, valid, width, height, depth);
    auto output_buffer = run_kernel_with_buffer<type> (arr_1, shape);
    if (IsDifferent (output_buffer, valid))
        {
            std::cerr << "kernel (" << width << "," << height << "," << depth << ") using buffer ... FAILED! " << std::endl;
            return true;
        }
    auto output_image = run_kernel_with_image<type> (arr_1, shape);
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
            std::cerr << "MinimumOfAllPixels kernel 3d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 5, 1))
        {
            std::cerr << "MinimumOfAllPixels kernel 2d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 1, 1))
        {
            std::cerr << "MinimumOfAllPixels kernel 1d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    std::cout << "MinimumOfAllPixels kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}