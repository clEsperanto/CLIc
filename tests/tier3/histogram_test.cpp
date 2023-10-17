#include <algorithm>
#include <random>

#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type>                         input(shape[0] * shape[1] * shape[2]);
  std::vector<type>                         valid(shape[0]);
  static std::uniform_int_distribution<int> distribution(0, shape[0] - 1);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<type>(distribution(generator)); });
  std::fill(valid.begin(), valid.end(), static_cast<type>(0.0F));
  for (auto && i : input)
  {
    valid[i] += static_cast<type>(1.0F);
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier3::histogram_func(device, gpu_input, nullptr, shape[0], 0, shape[0] - 1);

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