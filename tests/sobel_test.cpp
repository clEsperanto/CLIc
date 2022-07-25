
#include "cleUtils.hpp"
#include "clesperanto.hpp"
#include <random>

template <class type>
std::array<size_t, 3>
generate_data (std::vector<type> &arr_1,
               std::vector<type> &valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize (width * height * depth);
    valid.resize (width * height * depth);
    std::fill (arr_1.begin (), arr_1.end (), static_cast<type> (0));
    std::fill (valid.begin (), valid.end (), static_cast<type> (0));
    int index (0), central ((width / 2) + (height / 2) * width + (depth / 2) * height * width);

    int i = 1, j = 1;
    arr_1[central] = 1;
    if (width > 1)
        {
            i = (height == 1 && depth == 1) ? 4 : 2;
            j = (height > 1 && depth > 1) ? 2 : 1;
            valid[central - 1] = static_cast<type> (4 / i * j);
            valid[central + 1] = static_cast<type> (4 / i * j);
        }
    if (height > 1)
        {
            i = (depth == 1) ? 2 : 1;
            valid[central - width] = static_cast<type> (4 / i);
            valid[central + width] = static_cast<type> (4 / i);
            valid[central - width - 1] = static_cast<type> (sqrt (2) * (2 / i));
            valid[central - width + 1] = static_cast<type> (sqrt (2) * (2 / i));
            valid[central + width - 1] = static_cast<type> (sqrt (2) * (2 / i));
            valid[central + width + 1] = static_cast<type> (sqrt (2) * (2 / i));
        }
    if (depth > 1)
        {
            valid[central - (height * width)] = static_cast<type> (4);
            valid[central - (height * width) - 1] = static_cast<type> (sqrt (2) * 2);
            valid[central - (height * width) + 1] = static_cast<type> (sqrt (2) * 2);
            valid[central - (height * width) + width] = static_cast<type> (sqrt (2) * 2);
            valid[central - (height * width) - width] = static_cast<type> (sqrt (2) * 2);
            valid[central - (height * width) + width - 1] = static_cast<type> (sqrt (3));
            valid[central - (height * width) + width + 1] = static_cast<type> (sqrt (3));
            valid[central - (height * width) - width - 1] = static_cast<type> (sqrt (3));
            valid[central - (height * width) - width + 1] = static_cast<type> (sqrt (3));
            valid[central + (height * width)] = static_cast<type> (4);
            valid[central + (height * width) - 1] = static_cast<type> (sqrt (2) * 2);
            valid[central + (height * width) + 1] = static_cast<type> (sqrt (2) * 2);
            valid[central + (height * width) + width] = static_cast<type> (sqrt (2) * 2);
            valid[central + (height * width) - width] = static_cast<type> (sqrt (2) * 2);
            valid[central + (height * width) + width - 1] = static_cast<type> (sqrt (3));
            valid[central + (height * width) + width + 1] = static_cast<type> (sqrt (3));
            valid[central + (height * width) - width - 1] = static_cast<type> (sqrt (3));
            valid[central + (height * width) - width + 1] = static_cast<type> (sqrt (3));
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
    auto ocl_output = cle.Create<type> (shape);
    cle.Sobel (oclArray_A, ocl_output);
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
    auto ocl_output = cle.Create<type> (shape, "image");
    cle.Sobel (oclArray_A, ocl_output);
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
    if (test<float> (5, 5, 5))
        {
            std::cerr << "Sobel kernel 3d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (5, 5, 1))
        {
            std::cerr << "Sobel kernel 2d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    if (test<float> (5, 1, 1))
        {
            std::cerr << "Sobel kernel 1d ... FAILED! " << std::endl;
            return EXIT_FAILURE;
        }
    std::cout << "Sobel kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
