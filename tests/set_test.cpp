
#include <cassert>
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test (const std::array<size_t, 3> &shape, const cl_mem_object_type &mem_type) -> bool
{
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    static std::uniform_int_distribution<int> distribution (0, 100);
    static std::default_random_engine generator;
    std::generate (input.begin (), input.end (), [] () { return static_cast<type> (distribution (generator)); });
    std::fill (valid.begin (), valid.end (), 10);

    cle::Clesperanto cle;
    cle.GetDevice ()->WaitForKernelToFinish ();
    auto gpu_input = cle.Push<type> (input, shape, mem_type);
    cle.Set (gpu_input, 10);
    auto output = cle.Pull<type> (gpu_input);

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
    //    assert (run_test<int>({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned int>({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<short>({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned short>({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<char>({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned char>({ 5, 1, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<int>({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned int>({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<short>({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned short>({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<char>({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned char>({ 5, 3, 1 }, CL_MEM_OBJECT_IMAGE1D));
    assert (run_test<float> ({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<int>({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned int>({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<short>({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned short>({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<char>({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    //    assert (run_test<unsigned char>({ 5, 3, 2 }, CL_MEM_OBJECT_IMAGE1D));
    return EXIT_SUCCESS;
}