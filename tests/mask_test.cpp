#include "cle.hpp"

#include <assert.h>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input1(shape[0] * shape[1] * shape[2]);
  std::vector<type> input2(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);

  for (size_t i = 0; i < input1.size(); ++i)
  {
    input1[i] = static_cast<type>(rand() % 10);
  }
  std::fill(input2.begin(), input2.end(), static_cast<type>(0));
  std::fill(valid.begin(), valid.end(), static_cast<type>(0));
  const int center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[0] * shape[1];
  valid[center] = input1[center];
  input2[center] = static_cast<type>(1);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input1 = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  auto gpu_input2 = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::mask_func(device, gpu_input1, gpu_input2, nullptr);

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
  // assert(run_test<int64_t>({ 4, 3, 2 }, cle::mType::BUFFER) == 0);
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
