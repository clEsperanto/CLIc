
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
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

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input1 = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::add_images_weighted_func(device, gpu_input1, gpu_input2, nullptr, factor1, factor2);

  std::vector<type> output(gpu_output->size());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}