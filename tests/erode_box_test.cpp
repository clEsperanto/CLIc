
#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  const int         center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[0] * shape[1];
  input[center] = static_cast<type>(1);
  valid[center] = static_cast<type>(1);
  input[center + 1] = static_cast<type>(1);
  input[center - 1] = static_cast<type>(1);
  input[center + shape[0]] = static_cast<type>(1);
  input[center - shape[0]] = static_cast<type>(1);
  input[center + shape[0] + 1] = static_cast<type>(1);
  input[center - shape[0] - 1] = static_cast<type>(1);
  input[center + shape[0] - 1] = static_cast<type>(1);
  input[center - shape[0] + 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) + shape[0]] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) - 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) + 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) - shape[0]] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(1);
  input[center + (shape[1] * shape[0])] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) + shape[0]] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) - 1] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) + 1] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) - shape[0]] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(1);
  input[center - (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(1);
  input[center - (shape[1] * shape[0])] = static_cast<type>(1);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::erode_box_func(device, gpu_input, nullptr);

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

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}
