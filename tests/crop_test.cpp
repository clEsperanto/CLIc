
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape,
         const cle::MemoryType &       mem_type,
         const std::array<size_t, 3> & output_shape,
         const std::array<int, 3> &    crop_start) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::generate(input.begin(), input.end(), [&]() { return (static_cast<type>((1 + (rand() % 9)))); });

  std::vector<type> valid;
  for (size_t i = crop_start[2]; i < crop_start[2] + output_shape[2]; i++)
  {
    for (size_t j = crop_start[0]; j < crop_start[0] + output_shape[0]; j++)
    {
      for (size_t k = crop_start[1]; k < crop_start[1] + output_shape[1]; k++)
      {
        valid.push_back(input[i * shape[0] * shape[1] + j * shape[0] + k]);
      }
    }
  }

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(output_shape, mem_type);
  cle.Crop(gpu_input, gpu_output, crop_start[0], crop_start[1], crop_start[2]);
  auto output = cle.Pull<type>(gpu_output);

  return std::equal(output.begin(), output.end(), valid.begin());
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 10, 1, 1 }, cle::BUFFER, { 5, 1, 1 }, { 2, 0, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 6, 8, 1 }, cle::BUFFER, { 4, 5, 1 }, { 1, 2, 0 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 6, 8, 6 }, cle::BUFFER, { 3, 5, 3 }, { 2, 3, 1 }))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}