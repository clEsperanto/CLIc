#include "cle.hpp"

#include <assert.h>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);

  input = { 1, 0, 2, 1, 0, 1, 0, 3, 0, 2, 0, 1, 3, 0, 0, 1, 0, 1, 1, 0, 2, 0, 2, 1 };
  valid = { 4, -22, 30, -6, -21, 6, -26, 50, -19, 34, -28, -2, 50, -23, -18, 3, -27, 3, 5, -26, 31, -25, 28, -1 };


  auto device = cle::BackendManager::getInstance().getBackend().getDevice("TX", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::laplace_box_func(device, gpu_input, nullptr);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int64_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 4, 3, 2 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}
