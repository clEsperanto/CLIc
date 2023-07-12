
#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  const float       value1 = static_cast<float>(rand() % 4) + 1;
  const float       value2 = static_cast<float>(rand() % 2) + 1;
  std::vector<type> input1(shape[0] * shape[1] * shape[2]);
  std::vector<type> input2(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input1.begin(), input1.end(), static_cast<type>(value1));
  std::fill(input2.begin(), input2.end(), static_cast<type>(value2));
  std::fill(valid.begin(), valid.end(), static_cast<type>(value1 / value2));

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input1 = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::divide_images_func(device, gpu_input1, gpu_input2, nullptr);

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
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
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