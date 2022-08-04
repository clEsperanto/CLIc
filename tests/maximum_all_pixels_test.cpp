
#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    static std::uniform_int_distribution<int> distribution (1, 10);
    static std::default_random_engine generator;
    std::generate (input.begin (), input.end (), [] () { return static_cast<type> (distribution (generator)); });
    const int center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[0] * shape[1];
    input[center] = std::numeric_limits<type>::max ();

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    auto gpu_output = cle.Create<type> (shape, mem_type);
    cle.MaximumOfAllPixels (gpu_input, gpu_output);
    auto output = cle.Pull<type> (gpu_output).front ();

    std::cout << "max expected = " << std::numeric_limits<type>::max () << std::endl;
    std::cout << "max detect = " << output << std::endl;

    return output == std::numeric_limits<type>::max ();
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
    //    assert (run_test<int>({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned int>({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<short>({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned short>({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<char>({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned char>({ 10, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<int>({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned int>({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<short>({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned short>({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<char>({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned char>({ 10, 7, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<int>({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned int>({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<short>({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned short>({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<char>({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned char>({ 10, 7, 5 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}