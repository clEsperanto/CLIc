#include "cle.hpp"

#include <assert.h>
#include <random>

#include <iomanip>
#include <limits>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  static const size_t w = shape[0];
  static const size_t h = shape[1];
  static const size_t d = shape[2];
  std::vector<type>   input(w * h * d, static_cast<type>(0));
  std::vector<type>   valid(input.size(), static_cast<type>(0));
  const size_t        center = (w / 2) + (h / 2) * w + (d / 2) * h * w;
  input[center] = 100;
  // valid = { static_cast<type>(0.2915041744709014892578125F), static_cast<type>(1.30643117427825927734375F),
  //           static_cast<type>(2.1539404392242431640625F),    static_cast<type>(1.30643117427825927734375F),
  //           static_cast<type>(0.2915041744709014892578125F), static_cast<type>(1.3064310550689697265625F),
  //           static_cast<type>(5.855018138885498046875F),     static_cast<type>(9.6532917022705078125F),
  //           static_cast<type>(5.855018138885498046875F),     static_cast<type>(1.3064310550689697265625F),
  //           static_cast<type>(2.153940677642822265625F),     static_cast<type>(9.65329265594482421875F),
  //           static_cast<type>(15.91558742523193359375F),     static_cast<type>(9.65329265594482421875F),
  //           static_cast<type>(2.153940677642822265625F),     static_cast<type>(1.3064310550689697265625F),
  //           static_cast<type>(5.855018138885498046875F),     static_cast<type>(9.6532917022705078125F),
  //           static_cast<type>(5.855018138885498046875F),     static_cast<type>(1.3064310550689697265625F),
  //           static_cast<type>(0.2915041744709014892578125F), static_cast<type>(1.30643117427825927734375F),
  //           static_cast<type>(2.1539404392242431640625F),    static_cast<type>(1.30643117427825927734375F),
  //           static_cast<type>(0.2915041744709014892578125F) };

  valid = { static_cast<type>(0.2915041745), static_cast<type>(1.306431174),  static_cast<type>(2.153940439),
            static_cast<type>(1.306431174),  static_cast<type>(0.2915041745), static_cast<type>(1.306431055),
            static_cast<type>(5.855018139),  static_cast<type>(9.653291702),  static_cast<type>(5.855018139),
            static_cast<type>(1.306431055),  static_cast<type>(2.153940678),  static_cast<type>(9.653292656),
            static_cast<type>(15.91558743),  static_cast<type>(9.653292656),  static_cast<type>(2.153940678),
            static_cast<type>(1.306431055),  static_cast<type>(5.855018139),  static_cast<type>(9.653291702),
            static_cast<type>(5.855018139),  static_cast<type>(1.306431055),  static_cast<type>(0.2915041745),
            static_cast<type>(1.306431174),  static_cast<type>(2.153940439),  static_cast<type>(1.306431174),
            static_cast<type>(0.2915041745) };


  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  auto gpu_input = cle::Array::create(w, h, d, cle::toType<type>(), mem_type, input.data(), device);

  auto gpu_output = cle::tier1::gaussian_blur_func(device, gpu_input, nullptr, 1, 1, 1);

  std::vector<type> output(gpu_output->nbElements());
  gpu_output->read(output.data());

  // round values of valid vector values to 6 decimals to avoid float precision errors in comparison
  std::transform(valid.begin(), valid.end(), valid.begin(), [](type v) { return std::round(v * 100000); });
  std::transform(output.begin(), output.end(), output.begin(), [](type v) { return std::round(v * 100000); });

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}


auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 5, 1 }, cle::mType::BUFFER) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 5, 5, 1 }, cle::mType::BUFFER) == 0);

  return EXIT_SUCCESS;
}