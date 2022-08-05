

#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    if (shape[2] > 1)
        {
            input = { 0, 0, 0, 0, 0, 0,
                      0, 2, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 1, 0,
                      0, 0, 0, 0, 0, 0 };
            valid = { 2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1 };
        }
    else if (shape[1] > 1)
        {
            input = { 0, 0, 0, 0, 0, 0,
                      0, 2, 0, 0, 1, 0,
                      0, 0, 0, 0, 0, 0 };
            valid = { 2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1,
                      2, 2, 2, 1, 1, 1 };
        }
    else
        {
            input = { 0, 1, 0, 0, 2, 0 };
            valid = { 1, 1, 1, 2, 2, 2 };
        }

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    auto gpu_output = cle.Create<type> (shape, mem_type);
    cle.ExtendLabelingViaVoronoi (gpu_input, gpu_output);
    auto output = cle.Pull<type> (gpu_output);

    return std::equal (output.begin (), output.end (), valid.begin ());
}

auto
main (int argc, char **argv) -> int
{
    if (!run_test<float> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<int> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned int> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<short> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned short> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<char> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned char> ({ 6, 1, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<float> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<int> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned int> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<short> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned short> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<char> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned char> ({ 6, 3, 1 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<float> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<int> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned int> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<short> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned short> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<char> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<unsigned char> ({ 6, 3, 2 }, CL_MEM_OBJECT_BUFFER))
        return EXIT_FAILURE;
    if (!run_test<float> ({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D))
        return EXIT_FAILURE;
    //    if (!run_test<int>({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned int>({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<short>({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned short>({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<char>({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned char>({ 6, 1, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    if (!run_test<float> ({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D))
        return EXIT_FAILURE;
    //    if (!run_test<int>({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned int>({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<short>({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned short>({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<char>({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned char>({ 6, 3, 1 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    if (!run_test<float> ({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D))
        return EXIT_FAILURE;
    //    if (!run_test<int>({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned int>({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<short>({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned short>({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<char>({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    //    if (!run_test<unsigned char>({ 6, 3, 2 }, CL_MEM_OBJECT_IMAGE1D)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}