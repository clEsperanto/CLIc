#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(0));
  std::fill(valid.begin(), valid.end(), static_cast<type>(0));
  const size_t center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[1] * shape[0];
  type         d = 1;
  if (shape[0] > 1)
    d += 2.0f;
  if (shape[1] > 1)
    d += 2.0f;
  if (shape[2] > 1)
    d += 2.0f;
  input[center] = d;
  for (size_t i = 0; i < input.size(); ++i)
  {
    if ((i >= center - 1 && i <= center + 1) || (i == center + shape[0]) || (i == center - shape[0]) ||
        (i == center - (shape[1] * shape[0])) || (i == center + (shape[1] * shape[0])))
    {
      valid[i] = static_cast<type>(1);
    }
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::mean_sphere_func(device, gpu_input, nullptr, 1, 1, 1);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);

  // NOT AVAILABLE ON CUDA FOR NOW (TODO)
  // cle::BackendManager::getInstance().setBackend("cuda");
  // std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  // assert(run_test<float>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // // assert(run_test<int64_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int32_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int16_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int8_t>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);
  // // assert(run_test<int64_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // // assert(run_test<int32_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // // assert(run_test<int16_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);
  // // assert(run_test<int8_t>({ 10, 7, 5 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}