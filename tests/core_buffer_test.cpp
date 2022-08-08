

#include <iostream>
#include <vector>

#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"
#include "cleUtils.hpp"

template <class type>
auto
run_test(const std::shared_ptr<cle::Processor> & gpu, const std::array<size_t, 3> & shape) -> bool
{
  auto          object_type = cle::BUFFER;
  cle::DataType data_type{};
  data_type.Set<type>();

  const type        data = 10;
  std::vector<type> array(shape[0] * shape[1] * shape[2]);
  std::fill(array.begin(), array.end(), data);

  auto gpu_output = cle::Memory::AllocateObject(gpu, shape, data_type.Get(), object_type);
  auto gpu_input = cle::Memory::AllocateObject(gpu_output);
  cle::Memory::WriteObject(gpu_input, array);
  gpu_input.CopyDataTo(gpu_output);

  auto output = cle::Memory::ReadObject<type>(gpu_output);
  return std::equal(output.begin(), output.end(), array.begin());
}

auto
main(int argc, char ** argv) -> int
{
  auto gpu = std::make_shared<cle::Processor>();
  gpu->SelectDevice();
  gpu->WaitForKernelToFinish();

  if (!run_test<float>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<int>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<short>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<char>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned int>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned short>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned char>(gpu, { 10, 1, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<float>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<int>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<short>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<char>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned int>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned short>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned char>(gpu, { 10, 5, 1 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<float>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<int>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<short>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<char>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned int>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned short>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  if (!run_test<unsigned char>(gpu, { 10, 5, 3 }))
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}