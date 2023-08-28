
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);

  for (auto it = input.begin(); it != input.end(); ++it)
  {
    *it = static_cast<type>(rand() % 2);
  }
  for (auto it = input.begin(), it_valid = valid.begin(); (it != input.end()) && (it_valid != valid.end());
       ++it, ++it_valid)
  {
    if (*it != 0)
    {
      *it_valid = static_cast<type>((it_valid - valid.begin()) + 1);
    }
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::set_nonzero_pixels_to_pixelindex_func(device, gpu_input, nullptr, 1);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 3, 2 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 3, 2 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}