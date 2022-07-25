
#include "cleUtils.hpp"
#include "clesperanto.hpp"
#include <random>

template <class type>
std::array<size_t, 3>
generate_data (std::vector<type> &arr_1,
               std::vector<type> &valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize (width * height * depth);
    valid.resize (width);
    std::fill (valid.begin (), valid.end (), static_cast<type> (0));
    for (auto it1 = arr_1.begin (); it1 != arr_1.end (); ++it1)
        {
            int value = (int)rand () % width;
            if ((value % 2) == 0)
                {
                    *it1 = static_cast<type> (value);
                    valid[value] = static_cast<type> (1);
                }
            else
                {
                    *it1 = static_cast<type> (0);
                    valid[0] = static_cast<type> (1);
                }
        }
    return std::array<size_t, 3>{ width, height, depth };
}

template <class type>
std::vector<type>
run_kernel_with_buffer (std::vector<type> &arr_1, std::array<size_t, 3> &shape)
{
    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto oclArray_A = cle.Push<type> (arr_1, shape);
    auto ocl_output = cle.Create<type> ({ shape[0], 1, 1 });
    cle.FlagExistingLabels (oclArray_A, ocl_output);
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
    auto ocl_output = cle.Create<type> ({ shape[0], 1, 1 }, "image");
    cle.FlagExistingLabels (oclArray_A, ocl_output);
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
    if (test<float> (10, 5, 3))
        {
            std::cerr << "FlagExistingLabels kernel 3d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 5, 1))
        {
            std::cerr << "FlagExistingLabels kernel 2d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (10, 1, 1))
        {
            std::cerr << "FlagExistingLabels kernel 1d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    std::cout << "FlagExistingLabels kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
