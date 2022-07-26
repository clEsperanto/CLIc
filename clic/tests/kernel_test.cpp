
#include "cleAddImageAndScalarKernel.hpp"
#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleUtils.hpp"

auto
MemType (const std::string &type) -> cl_mem_object_type
{
    if (type.find ("buffer") != std::string::npos)
        {
            return CL_MEM_OBJECT_BUFFER;
        }
    if (type.find ("image") != std::string::npos)
        {
            return CL_MEM_OBJECT_IMAGE1D;
        }
    throw std::runtime_error ("");
};

template <class type>
auto
run_test (std::array<size_t, 3> shape, const cl_mem_object_type &mem_type) -> bool
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
    auto gpu_output = cle::Memory::AllocateObject (gpu, shape, data_type.Get (), mem_type);
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
    std::string data_t = argv[++idx];
    std::string type_t = argv[++idx];
    size_t w = std::atoi (argv[++idx]);
    size_t h = std::atoi (argv[++idx]);
    size_t d = std::atoi (argv[++idx]);

    std::array<size_t, 3> shape = { w, h, d };
    if (data_t == "float")
        {
            assert (run_test<float> (shape, MemType (type_t)));
        }
    if (data_t == "int")
        {
            assert (run_test<int> (shape, MemType (type_t)));
        }
    if (data_t == "short")
        {
            assert (run_test<short> (shape, MemType (type_t)));
        }
    if (data_t == "char")
        {
            assert (run_test<char> (shape, MemType (type_t)));
        }
    if (data_t == "uint")
        {
            assert (run_test<unsigned int> (shape, MemType (type_t)));
        }
    if (data_t == "ushort")
        {
            assert (run_test<unsigned short> (shape, MemType (type_t)));
        }
    if (data_t == "uchar")
        {
            assert (run_test<unsigned char> (shape, MemType (type_t)));
        }
    std::cout << "all is good\n";
    return EXIT_SUCCESS;
}