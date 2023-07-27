
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);

  input = {
    static_cast<type>(1.0), static_cast<type>(2.0),  static_cast<type>(3.0),  static_cast<type>(4.0),
    static_cast<type>(5.0), static_cast<type>(6.0),  static_cast<type>(7.0),  static_cast<type>(8.0),
    static_cast<type>(9.0), static_cast<type>(10.0), static_cast<type>(11.0), static_cast<type>(12.0)
  }; // 6.0058594 skimage
  valid = {
    static_cast<type>(0.0), static_cast<type>(0.0), static_cast<type>(0.0), static_cast<type>(0.0),
    static_cast<type>(0.0), static_cast<type>(0.0), static_cast<type>(1.0), static_cast<type>(1.0),
    static_cast<type>(1.0), static_cast<type>(1.0), static_cast<type>(1.0), static_cast<type>(1.0)
  }; // 6.00392 clic

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  for (auto && i : input)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
  for (auto && i : valid)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
  for (auto && i : output)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;


  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 3, 2, 2 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 3, 2, 2 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}