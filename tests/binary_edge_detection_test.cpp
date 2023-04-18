

#include "clesperanto.hpp"

#include <algorithm>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  if (shape[2] > 1)
  {
    input = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
    valid = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
  }
  else if (shape[1] > 1)
  {
    input = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  }
  else
  {
    input = { 0, 0, 1, 1, 1, 1, 0, 0 };
    valid = { 0, 0, 1, 0, 0, 1, 0, 0 };
  }

  // print input
  std::cout << "input: " << std::endl;
  for (auto && i : input)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  // print valid
  std::cout << "valid: " << std::endl;
  for (auto && i : valid)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;


  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.BinaryEdgeDetection(gpu_input, gpu_output);
  auto output = cle.Pull<type>(gpu_output);

  // print output
  std::cout << "output: " << std::endl;
  for (auto && i : output)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  return std::equal(output.begin(), output.end(), valid.begin());
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 8, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 5, 6, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 6, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  // if (!run_test<float>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 8, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 5, 6, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 6, 5, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  return EXIT_SUCCESS;
}