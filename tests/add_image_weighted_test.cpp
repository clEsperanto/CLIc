
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::ObjectType & mem_type) -> bool
{
  std::vector<type> input1(shape[0] * shape[1] * shape[2]);
  std::vector<type> input2(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  const type        value_1 = 25;
  const type        value_2 = 75;
  const float       factor1 = 0.5;
  const float       factor2 = 0.25;
  std::fill(input1.begin(), input1.end(), value_1);
  std::fill(input2.begin(), input2.end(), value_2);
  std::fill(valid.begin(), valid.end(), static_cast<type>(value_1 * factor1 + value_2 * factor2));

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input1 = cle.Push<type>(input1, shape, mem_type);
  auto gpu_input2 = cle.Push<type>(input2, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.AddImagesWeighted(gpu_input1, gpu_input2, gpu_output, factor1, factor2);
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

  if (!run_test<float>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 10, 5, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned int>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed short>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned short>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed char>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<unsigned char>({ 10, 5, 3 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 10, 1, 1 }, cle::IMAGE))
  {
    return EXIT_FAILURE;
  }

  //  if (!run_test<signed int>({ 10, 1, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned int>({ 10, 1, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<signed short>({ 10, 1, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned short>({ 10, 1, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<signed char>({ 10, 1, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned char>({ 10, 1, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  if (!run_test<float>({ 10, 5, 1 }, cle::IMAGE))
  {
    return EXIT_FAILURE;
  }

  //  if (!run_test<signed int>({ 10, 5, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned int>({ 10, 5, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<signed short>({ 10, 5, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned short>({ 10, 5, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<signed char>({ 10, 5, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned char>({ 10, 5, 1 }, cle::IMAGE)) { return EXIT_FAILURE; }

  if (!run_test<float>({ 10, 5, 3 }, cle::IMAGE))
  {
    return EXIT_FAILURE;
  }

  //  if (!run_test<signed int>({ 10, 5, 3 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned int>({ 10, 5, 3 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<signed short>({ 10, 5, 3 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned short>({ 10, 5, 3 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<signed char>({ 10, 5, 3 }, cle::IMAGE)) { return EXIT_FAILURE; }

  //  if (!run_test<unsigned char>({ 10, 5, 3 }, cle::IMAGE)) { return EXIT_FAILURE; }

  return EXIT_SUCCESS;
}