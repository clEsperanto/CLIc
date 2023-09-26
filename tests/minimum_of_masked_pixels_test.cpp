
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> mask(shape[0] * shape[1] * shape[2]);

  for (size_t i = 0; i < shape[0] * shape[1] * shape[2]; i++)
  {
    input[i] = 100;
    mask[i] = (i % 2 == 0) ? 1 : 0;
  }
  input[2] = 3;
  float valid = 3;

  for (auto && i : input)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  for (auto && i : mask)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  auto gpu_mask = cle::Array::create(gpu_input);
  gpu_input->write(input.data());
  gpu_mask->write(mask.data());
  auto output = cle::tier2::minimum_of_masked_pixels_func(device, gpu_input, gpu_mask);

  std::cout << "GPU output: " << output << std::endl;
  std::cout << "valid output: " << valid << std::endl;

  // std::vector<type> output(gpu_output->size());
  // gpu_output->read(output.data());

  return output == valid ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 3, 2 }, cle::mType::BUFFER) == 0);

  // cle::BackendManager::getInstance().setBackend("cuda");
  // std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  // assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}