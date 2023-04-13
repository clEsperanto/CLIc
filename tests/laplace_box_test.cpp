

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);

  if (shape[2] > 1)
  {
    input = { 1, 0, 2, 1, 0, 1, 0, 3, 0, 2, 0, 1, 3, 0, 0, 1, 0, 1, 1, 0, 2, 0, 2, 1 };
    valid = { 3, -4, 11, 0, -2, 3, -7, 16, -4, 11, -5, 2, 17, -4, -4, 5, -6, 4, 3, -6, 12, -7, 10, 3 };
  }
  else if (shape[1] > 1)
  {
    input = { 1, 0, 2, 1, 0, 1, 3, 1, 0, 2, 1, 1 };
    valid = { 7, -7, 10, 2, -4, -1, 15, 0, -3, 11, 0, 3 };
  }
  else
  {
    input = { 1, 0, 0, 1, 1 };
    valid = { 2, -1, -1, 1, 1 };
  }

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.LaplaceBox(gpu_input, gpu_output);
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

  if (!run_test<int16_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 5, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 3, 4, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 3, 4, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 3, 4, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 3, 4, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 3, 4, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 3, 4, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 3, 4, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 3, 4, 2 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}