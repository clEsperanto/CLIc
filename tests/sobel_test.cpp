#include "cle.hpp"

#include <assert.h>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(0));
  std::fill(valid.begin(), valid.end(), static_cast<type>(0));

  int center((shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[1] * shape[0]);

  int i = 1, j = 1;
  input[center] = 1;
  i = (shape[1] == 1 && shape[2] == 1) ? 4 : 2;
  j = (shape[1] > 1 && shape[2] > 1) ? 2 : 1;
  valid[center - 1] = static_cast<type>(4 / i * j);
  valid[center + 1] = static_cast<type>(4 / i * j);
  i = (shape[2] == 1) ? 2 : 1;
  valid[center - shape[0]] = static_cast<type>(4 / i);
  valid[center + shape[0]] = static_cast<type>(4 / i);
  valid[center - shape[0] - 1] = static_cast<type>(sqrt(2) * (2 / i));
  valid[center - shape[0] + 1] = static_cast<type>(sqrt(2) * (2 / i));
  valid[center + shape[0] - 1] = static_cast<type>(sqrt(2) * (2 / i));
  valid[center + shape[0] + 1] = static_cast<type>(sqrt(2) * (2 / i));
  valid[center - (shape[1] * shape[0])] = static_cast<type>(4);
  valid[center - (shape[1] * shape[0]) - 1] = static_cast<type>(sqrt(2) * 2);
  valid[center - (shape[1] * shape[0]) + 1] = static_cast<type>(sqrt(2) * 2);
  valid[center - (shape[1] * shape[0]) + shape[0]] = static_cast<type>(sqrt(2) * 2);
  valid[center - (shape[1] * shape[0]) - shape[0]] = static_cast<type>(sqrt(2) * 2);
  valid[center - (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(sqrt(3));
  valid[center - (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(sqrt(3));
  valid[center - (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(sqrt(3));
  valid[center - (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(sqrt(3));
  valid[center + (shape[1] * shape[0])] = static_cast<type>(4);
  valid[center + (shape[1] * shape[0]) - 1] = static_cast<type>(sqrt(2) * 2);
  valid[center + (shape[1] * shape[0]) + 1] = static_cast<type>(sqrt(2) * 2);
  valid[center + (shape[1] * shape[0]) + shape[0]] = static_cast<type>(sqrt(2) * 2);
  valid[center + (shape[1] * shape[0]) - shape[0]] = static_cast<type>(sqrt(2) * 2);
  valid[center + (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(sqrt(3));
  valid[center + (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(sqrt(3));
  valid[center + (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(sqrt(3));
  valid[center + (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(sqrt(3));

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::sobel_func(device, gpu_input, nullptr);

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

  // not compatible with CUDA for now
  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 7, 5 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}