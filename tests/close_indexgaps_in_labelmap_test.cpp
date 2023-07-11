
#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  const type        offset = 2;
  for (auto i = 0; i < valid.size(); i++)
  {
    valid[i] = i + 1;
    input[i] = i + 1 + offset;
  }

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.CloseIndexGapsInLabelMap(gpu_input, gpu_output);
  auto output = cle.Pull<type>(gpu_output);

  return std::equal(output.begin(), output.end(), valid.begin());
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 5, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 5, 3, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }


  // if (!run_test<float>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 5, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 5, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 5, 3, 2 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  return EXIT_SUCCESS;
}