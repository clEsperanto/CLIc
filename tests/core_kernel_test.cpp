
#include "cleAddImageAndScalarKernel.hpp"
#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"


auto
MemType(const std::string & type) -> cle::MemoryType
{
  if (type.find("buffer") != std::string::npos)
  {
    return cle::BUFFER;
  }
  if (type.find("image") != std::string::npos)
  {
    return cle::IMAGE1D;
  }
  throw std::runtime_error("");
};

template <class type>
auto
run_test(const std::shared_ptr<cle::Processor> & gpu, std::array<size_t, 3> shape, const cle::MemoryType & mem_type)
  -> bool
{
  const type base = 10;
  const type add = 15;

  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), base);
  std::fill(valid.begin(), valid.end(), base + add);

  cle::DataType data_type = cle::TypeToDataType<type>();
  auto          gpu_output = cle::Memory::AllocateMemory(gpu, shape, data_type, mem_type);
  auto          gpu_intput = cle::Memory::AllocateMemory(gpu_output);

  cle::Memory::WriteObject(gpu_intput, input);

  cle::AddImageAndScalarKernel kernel(gpu);
  kernel.SetInput(gpu_intput);
  kernel.SetOutput(gpu_output);
  kernel.SetScalar(add);
  kernel.Execute();

  std::cout << kernel << std::endl;


  auto output = cle::Memory::ReadObject<type>(gpu_output);
  return std::equal(output.begin(), output.end(), valid.begin());
}

int
main(int argc, char ** argv)
{
  auto gpu = std::make_shared<cle::Processor>();
  gpu->SelectDevice();
  gpu->WaitForKernelToFinish();

  std::string type_t = argv[1];
  if (!run_test<float>(gpu, { 10, 1, 1 }, MemType(type_t)))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>(gpu, { 10, 5, 1 }, MemType(type_t)))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>(gpu, { 10, 5, 3 }, MemType(type_t)))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}