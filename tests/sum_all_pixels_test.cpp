#include "cle.hpp"

#include <assert.h>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(1));

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto output = cle::tier2::sum_of_all_pixels_func(device, gpu_input);

  return (static_cast<type>(output) == static_cast<type>(shape[0] * shape[1] * shape[2])) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  assert(run_test<float>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 5, 5, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 5, 5, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 5, 5, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 5, 5, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 5, 5, 2 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  assert(run_test<float>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}