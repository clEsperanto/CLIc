#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
  std::vector<type> valid = { 0, 0, 0, 1, 0, -1, 0, 0, 0 };

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("TX", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::gradient_x_func(device, gpu_input, nullptr);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int64_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 9, 1, 1 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 9, 1, 1 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}
