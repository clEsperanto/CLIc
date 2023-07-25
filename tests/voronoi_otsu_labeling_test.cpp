
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  input = { 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 3, 1, 0, 0, 0, 1,
            5, 8, 2, 0, 0, 0, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 0, 0, 0, 2, 8, 5, 1, 0, 0,
            0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 };
  valid = { 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 2, 2,
            2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,
            1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0,
            0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2,
            2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0 };

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier7::voronoi_otsu_labeling_func(device, gpu_input, nullptr, 0, 1);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 7, 7, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 7, 7, 3 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}