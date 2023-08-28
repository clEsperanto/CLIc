
#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  for (auto it = input.begin(), it_valid = valid.begin(); (it != input.end()) && (it_valid != valid.end());
       ++it, ++it_valid)
  {
    *it = static_cast<type>((int)rand() % 10);
    *it_valid = *it;
    if ((it - input.begin()) % shape[0] == 1)
    {
      *it_valid = static_cast<type>(100);
    }
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  cle::tier1::set_column_func(device, gpu_input, 1, 100);

  std::vector<type> output(gpu_input->nbElements());
  gpu_input->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<uint8_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<uint8_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}