#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  type              index = 0;
  for (auto i = 0; i < valid.size(); i++)
  {
    if (i % 2 == 0)
    {
      valid[i] = ++index;
      input[i] = 1;
    }
    else
    {
      valid[i] = 0;
      input[i] = 0;
    }
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("TX", "all");
  int  blocksize = 4;
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  size_t block_value = static_cast<size_t>((static_cast<size_t>(shape[0] - 1) + 1) / blocksize) + 1;
  auto   gpu_temp = cle::Array::create(block_value, 1, 1, cle::toType<type>(), mem_type, device);

  cle::tier1::sum_reduction_x_func(device, gpu_input, gpu_temp, blocksize);
  auto gpu_output = cle::tier1::block_enumerate_func(device, gpu_input, gpu_temp, nullptr, blocksize);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int64_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 5, 1, 1 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 5, 1, 1 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}