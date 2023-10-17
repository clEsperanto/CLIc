
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  input = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 2, 3, 4, 0, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  valid = { 1, 2, 3, 3, 3, 2, 1, 2, 3, 4, 4, 2, 3, 4, 5, 4, 4, 4, 5, 5, 4, 4, 5, 5, 5,
            1, 2, 3, 3, 3, 2, 3, 4, 4, 4, 4, 4, 5, 5, 5, 4, 4, 5, 5, 5, 4, 4, 5, 5, 5 };

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());
  auto flag = cle::Array::create(1, 1, 1, cle::toType<type>(), mem_type, device);
  flag->fill(0);

  auto gpu_output = cle::tier1::onlyzero_overwrite_maximum_box_func(device, gpu_input, flag, nullptr);

  std::vector<type> output(gpu_output->size());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 5, 2 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}