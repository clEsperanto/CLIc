#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input1(shape[0] * shape[1] * shape[2]);
  std::vector<type> input2(shape[0]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  type              value = 0;
  for (auto it = input2.begin(); it != input2.end(); ++it)
  {
    *it = static_cast<type>((int)rand() % 10 + 10);
  }
  for (auto it = input1.begin(), it_valid = valid.begin(); (it != input1.end()) && (it_valid != valid.end());
       ++it, ++it_valid)
  {
    value = static_cast<type>((int)rand() % 10);
    *it = value;
    *it_valid = static_cast<type>(input2[value]);
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input1 = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  auto gpu_input2 = cle::Array::create(shape[0], 1, 1, cle::toType<type>(), mem_type, device);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::replace_intensities_func(device, gpu_input1, gpu_input2, nullptr);

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

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}