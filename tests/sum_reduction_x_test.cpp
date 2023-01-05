

#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid((shape[0] / 4) * shape[1] * shape[2]);
  input = { static_cast<type>(0.0), static_cast<type>(1.0), static_cast<type>(0.0), static_cast<type>(1.0),
            static_cast<type>(0.0), static_cast<type>(0.0), static_cast<type>(1.0), static_cast<type>(1.0),
            static_cast<type>(0.0), static_cast<type>(0.0), static_cast<type>(1.0), static_cast<type>(0.0) };
  valid = { static_cast<type>(2.0), static_cast<type>(2.0), static_cast<type>(1.0) };

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  // auto gpu_output = cle.Create<type>({ static_cast<size_t>(shape[0] / 4), 1, 1 }, mem_type);
  // cle.SumReductionX(gpu_input, gpu_output, 4);
  // auto output = cle.Pull<type>(gpu_output);

  // return std::equal(output.begin(), output.end(), valid.begin());
  return EXIT_FAILURE;
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 12, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  // if (!run_test<int32_t>({ 10, 7, 1 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 10, 7, 1 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 10, 7, 1 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 10, 7, 1 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 10, 7, 1 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 10, 7, 1 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 5, 3, 2 }, cle::BUFFER))
  // {
  //   return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 12, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 10, 7, 1 }, cle::IMAGE))
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