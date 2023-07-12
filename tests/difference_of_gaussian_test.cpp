#include "cle.hpp"

#include <algorithm>
#include <assert.h>
#include <random>

#include <iomanip>
#include <limits>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  std::vector<float> input(shape[0] * shape[1] * shape[2]);
  std::vector<type>  valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), 0.0F);
  std::fill(valid.begin(), valid.end(), static_cast<type>(0.0F));
  const int center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[1] * shape[0];
  input[center] = 100.0F;
  valid = { static_cast<type>(1.217670321), static_cast<type>(2.125371218), static_cast<type>(1.217670321),
            static_cast<type>(2.125371456), static_cast<type>(3.62864542),  static_cast<type>(2.125371456),
            static_cast<type>(1.217670321), static_cast<type>(2.125371218), static_cast<type>(1.217670321),
            static_cast<type>(2.125371456), static_cast<type>(3.62864542),  static_cast<type>(2.125371456),
            static_cast<type>(3.628645658), static_cast<type>(6.114237785), static_cast<type>(3.628645658),
            static_cast<type>(2.125371456), static_cast<type>(3.62864542),  static_cast<type>(2.125371456),
            static_cast<type>(1.217670321), static_cast<type>(2.125371218), static_cast<type>(1.217670321),
            static_cast<type>(2.125371456), static_cast<type>(3.62864542),  static_cast<type>(2.125371456),
            static_cast<type>(1.217670321), static_cast<type>(2.125371218), static_cast<type>(1.217670321) };

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier2::difference_of_gaussian_func(device, gpu_input, nullptr, 1, 1, 1, 3, 3, 3);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  // for (auto && i : output)
  // {
  //   // print i with maximum precision
  //   std::cout << std::setprecision(std::numeric_limits<type>::max_digits10 + 1) << i << " ";
  // }
  // std::cout << std::endl;

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int32_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int16_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int8_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 3,3,3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int32_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int16_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int8_t>({ 3, 3, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 3,3,3 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}
