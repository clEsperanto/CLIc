

#include "clesperanto.hpp"

#include <algorithm>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  std::vector<float> input(shape[0] * shape[1] * shape[2]);
  std::vector<type>  valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), 0.0F);
  std::fill(valid.begin(), valid.end(), static_cast<type>(0.0F));
  const int center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[1] * shape[0];
  input[center] = 100.0F;
  if (valid.size() == 27)
    valid = { static_cast<type>(1.217670320F), static_cast<type>(2.125371220F), static_cast<type>(1.217670320F),
              static_cast<type>(2.125371460F), static_cast<type>(3.628645420F), static_cast<type>(2.125371460F),
              static_cast<type>(1.217670320F), static_cast<type>(2.125371220F), static_cast<type>(1.217670320F),
              static_cast<type>(2.125371460F), static_cast<type>(3.628645420F), static_cast<type>(2.125371460F),
              static_cast<type>(3.628645660F), static_cast<type>(6.114237790F), static_cast<type>(3.628645660F),
              static_cast<type>(2.125371460F), static_cast<type>(3.628645420F), static_cast<type>(2.125371460F),
              static_cast<type>(1.217670320F), static_cast<type>(2.125371220F), static_cast<type>(1.217670320F),
              static_cast<type>(2.125371460F), static_cast<type>(3.628645420F), static_cast<type>(2.125371460F),
              static_cast<type>(1.217670320F), static_cast<type>(2.125371220F), static_cast<type>(1.217670320F) };

  if (valid.size() == 9)
    valid = { static_cast<type>(4.272504810F), static_cast<type>(7.980373380F), static_cast<type>(4.272504810F),
              static_cast<type>(7.980374340F), static_cast<type>(14.14709850F), static_cast<type>(7.980374340F),
              static_cast<type>(4.272504810F), static_cast<type>(7.980373380F), static_cast<type>(4.272504810F) };

  if (valid.size() == 3)
    valid = { static_cast<type>(11.61734770F), static_cast<type>(26.59589000F), static_cast<type>(11.61734770F) };

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<float>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.DifferenceOfGaussian(gpu_input, gpu_output, 1, 1, 1, 3, 3, 3);
  auto output = cle.Pull<type>(gpu_output);

  // !how can we improve float accuracy between gpu and cpu
  std::transform(output.begin(), output.end(), output.begin(), [](const type & x) {
    return static_cast<type>(std::floor(x * 100));
  });
  std::transform(
    valid.begin(), valid.end(), valid.begin(), [](const type & x) { return static_cast<type>(std::floor(x * 100)); });

  return std::equal(output.begin(), output.end(), valid.begin());
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 3, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 3, 3, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 3, 3, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  // if (!run_test<float>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed int>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned int>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed short>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned short>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed char>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned char>({ 3, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed int>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned int>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed short>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned short>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed char>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned char>({ 3, 3, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed int>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned int>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed short>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned short>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed char>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned char>({ 3, 3, 3 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  return EXIT_SUCCESS;
}
