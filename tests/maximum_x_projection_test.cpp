#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(1 * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(0));
  std::fill(valid.begin(), valid.end(), static_cast<type>(10));
  for (auto it = input.begin(); it != input.end(); std::advance(it, shape[0]))
  {
    int idx = (it - input.begin()) + (rand() % shape[0]);
    input[idx] = static_cast<type>(10);
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("TX", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::maximum_x_projection_func(device, gpu_input, nullptr);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}