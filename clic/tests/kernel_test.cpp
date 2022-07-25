
#include "cleKernelList.hpp"
#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleUtils.hpp"

template <class type>
auto
run_test (std::array<size_t, 3> shape, const cl_mem_object_type &format) -> bool
{
    const type base = 10;
    const type add = 15;
    std::vector<type> input (shape[0] * shape[1] * shape[2]);
    std::vector<type> valid (shape[0] * shape[1] * shape[2]);
    std::fill (input.begin (), input.end (), base);
    std::fill (valid.begin (), valid.end (), base + add);

    auto gpu = std::make_shared<cle::Processor> ();
    gpu->SelectDevice ();
    gpu->WaitForKernelToFinish ();

    cle::DataType data_type{};
    data_type.Set<type> ();

    auto gpu_output = cle::Memory::AllocateObject (gpu, shape, data_type.Get (), format);
    auto gpu_intput = cle::Memory::AllocateObject (gpu_output);
    cle::Memory::WriteObject (gpu_intput, input);

    cle::AddImageAndScalarKernel kernel (gpu);
    kernel.SetInput (gpu_intput);
    kernel.SetOutput (gpu_output);
    kernel.SetScalar (add);
    kernel.Execute ();

    auto output = cle::Memory::ReadObject<type> (gpu_output);

    return std::equal (output.begin (), output.end (), valid.begin ());
}

int
main (int argc, char **argv)
{
    int idx = 0;
    size_t w = std::atoi (argv[++idx]);
    size_t h = std::atoi (argv[++idx]);
    size_t d = std::atoi (argv[++idx]);

    std::array<size_t, 3> shape = { w, h, d };

    {
        using type = float;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
    {
        using type = int;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
    {
        using type = char;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        // assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
    {
        using type = short;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        // assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
    {
        using type = unsigned int;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
    {
        using type = unsigned char;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        // assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
    {
        using type = unsigned short;
        assert (run_test<type> (shape, CL_MEM_OBJECT_BUFFER));
        // assert (run_test<type> (shape, CL_MEM_OBJECT_IMAGE2D));
    }
}