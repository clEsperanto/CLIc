
#include <cassert>
#include <iostream>
#include <vector>

#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleUtils.hpp"

template <class type>
auto
run_test (std::shared_ptr<cle::Processor> gpu, std::array<size_t, 3> shape) -> bool
{
    auto object_type = CL_MEM_OBJECT_BUFFER;

    cle::DataType data_type{};
    data_type.Set<type> ();

    const type data = 10;
    std::vector<type> array (shape[0] * shape[1] * shape[2]);
    std::fill (array.begin (), array.end (), data);

    auto gpu_output = cle::Memory::AllocateObject (gpu, shape, data_type.Get (), object_type);
    auto gpu_input = cle::Memory::AllocateObject (gpu_output);
    cle::Memory::WriteObject (gpu_input, array);
    gpu_input.CopyDataTo (gpu_output);

    std::cout << "gpu buffer : " << gpu_output.ToString () << std::endl;

    auto output = cle::Memory::ReadObject<type> (gpu_output);

    return std::equal (output.begin (), output.end (), array.begin ());
}

auto
main (int argc, char **argv) -> int
{
    int idx = 0;
    size_t w = std::atoi (argv[++idx]);
    size_t h = std::atoi (argv[++idx]);
    size_t d = std::atoi (argv[++idx]);

    std::array<size_t, 3> shape = { w, h, d };

    auto gpu = std::make_shared<cle::Processor> ();
    gpu->SelectDevice ();
    gpu->WaitForKernelToFinish ();

    assert (run_test<float> (gpu, shape));
    assert (run_test<int> (gpu, shape));
    assert (run_test<char> (gpu, shape));
    assert (run_test<short> (gpu, shape));
    assert (run_test<unsigned int> (gpu, shape));
    assert (run_test<unsigned char> (gpu, shape));
    assert (run_test<unsigned short> (gpu, shape));

    return EXIT_SUCCESS;
}