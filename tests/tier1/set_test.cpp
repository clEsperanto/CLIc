#include "cle.hpp"

#include <algorithm>
#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type>                         input(shape[0] * shape[1] * shape[2]);
  std::vector<type>                         valid(shape[0] * shape[1] * shape[2]);
  static std::uniform_int_distribution<int> distribution(0, 100);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<type>(distribution(generator)); });
  std::fill(valid.begin(), valid.end(), static_cast<type>(10.0F));

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  cle::tier1::set_func(device, gpu_input, 10);

  std::vector<type> output(gpu_input->size());
  gpu_input->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<uint32_t>({ 5, 4, 3 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<uint32_t>({ 5, 4, 3 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}