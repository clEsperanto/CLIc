

#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(0));
  std::fill(valid.begin(), valid.end(), static_cast<type>(0));

  int index(0);
  int center((shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[1] * shape[0]);

  int i = 1, j = 1;
  input[center] = 1;
  if (shape[0] > 1)
  {
    i = (shape[1] == 1 && shape[2] == 1) ? 4 : 2;
    j = (shape[1] > 1 && shape[2] > 1) ? 2 : 1;
    valid[center - 1] = static_cast<type>(4 / i * j);
    valid[center + 1] = static_cast<type>(4 / i * j);
  }
  if (shape[1] > 1)
  {
    i = (shape[2] == 1) ? 2 : 1;
    valid[center - shape[0]] = static_cast<type>(4 / i);
    valid[center + shape[0]] = static_cast<type>(4 / i);
    valid[center - shape[0] - 1] = static_cast<type>(sqrt(2) * (2 / i));
    valid[center - shape[0] + 1] = static_cast<type>(sqrt(2) * (2 / i));
    valid[center + shape[0] - 1] = static_cast<type>(sqrt(2) * (2 / i));
    valid[center + shape[0] + 1] = static_cast<type>(sqrt(2) * (2 / i));
  }
  if (shape[2] > 1)
  {
    valid[center - (shape[1] * shape[0])] = static_cast<type>(4);
    valid[center - (shape[1] * shape[0]) - 1] = static_cast<type>(sqrt(2) * 2);
    valid[center - (shape[1] * shape[0]) + 1] = static_cast<type>(sqrt(2) * 2);
    valid[center - (shape[1] * shape[0]) + shape[0]] = static_cast<type>(sqrt(2) * 2);
    valid[center - (shape[1] * shape[0]) - shape[0]] = static_cast<type>(sqrt(2) * 2);
    valid[center - (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(sqrt(3));
    valid[center - (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(sqrt(3));
    valid[center - (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(sqrt(3));
    valid[center - (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(sqrt(3));
    valid[center + (shape[1] * shape[0])] = static_cast<type>(4);
    valid[center + (shape[1] * shape[0]) - 1] = static_cast<type>(sqrt(2) * 2);
    valid[center + (shape[1] * shape[0]) + 1] = static_cast<type>(sqrt(2) * 2);
    valid[center + (shape[1] * shape[0]) + shape[0]] = static_cast<type>(sqrt(2) * 2);
    valid[center + (shape[1] * shape[0]) - shape[0]] = static_cast<type>(sqrt(2) * 2);
    valid[center + (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(sqrt(3));
    valid[center + (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(sqrt(3));
    valid[center + (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(sqrt(3));
    valid[center + (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(sqrt(3));
  }

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.Sobel(gpu_input, gpu_output);
  auto output = cle.Pull<type>(gpu_output);

  return std::equal(output.begin(), output.end(), valid.begin());
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  // if (!run_test<float>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed int>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned int>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed short>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned short>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed char>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned char>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed int>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned int>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed short>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned short>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed char>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned char>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed int>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned int>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed short>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned short>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<signed char>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<unsigned char>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  return EXIT_SUCCESS;
}
