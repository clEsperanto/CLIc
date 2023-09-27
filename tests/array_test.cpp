
#include <assert.h>

#include "cle.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  {
    std::vector<type> input(shape[0]);
    std::fill(input.begin(), input.end(), static_cast<type>(-7));
    std::cout << "test 1d: create, write, copy, read: ... ";
    auto gpu_input = cle::Array::create(shape[0], 1, 1, cle::toType<type>(), mem_type, device);
    gpu_input->write(input.data());
    auto gpu_copy = cle::Array::create(gpu_input);
    gpu_input->copy(gpu_copy);
    std::vector<type> input_test(gpu_input->size());
    gpu_input->read(input_test.data());
    std::equal(input_test.begin(), input_test.end(), input.begin()) ? std::cout << "all good\n"
                                                                    : std::cout << "not good\n";
  }
  {
    std::vector<type> input(shape[0] * shape[1]);
    std::fill(input.begin(), input.end(), static_cast<type>(-7));
    std::cout << "test 2d: create, write, copy, read: ... ";
    auto gpu_input = cle::Array::create(shape[0], shape[1], 1, cle::toType<type>(), mem_type, device);
    gpu_input->write(input.data());
    auto gpu_copy = cle::Array::create(gpu_input);
    gpu_input->copy(gpu_copy);
    std::vector<type> input_test(gpu_copy->size());
    gpu_copy->read(input_test.data());
    std::equal(input_test.begin(), input_test.end(), input.begin()) ? std::cout << "all good\n"
                                                                    : std::cout << "not good\n";
  }
  {
    std::vector<type> input(shape[0] * shape[1] * shape[2]);
    std::fill(input.begin(), input.end(), static_cast<type>(-7));
    std::cout << "test 3d: create, write, copy, read: ... ";
    auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
    gpu_input->write(input.data());
    auto gpu_copy = cle::Array::create(gpu_input);
    gpu_input->copy(gpu_copy);
    std::vector<type> input_test(gpu_copy->size());
    gpu_copy->read(input_test.data());
    std::equal(input_test.begin(), input_test.end(), input.begin()) ? std::cout << "all good\n"
                                                                    : std::cout << "not good\n";
  }
  {
    type input = -7;
    std::cout << "test one: create, write, copy, read: ... ";
    auto gpu_input = cle::Array::create(1, 1, 1, cle::toType<type>(), mem_type, device);
    gpu_input->write(&input);
    auto gpu_copy = cle::Array::create(gpu_input);
    gpu_input->copy(gpu_copy);
    type input_test;
    gpu_copy->read(&input_test);
    (input_test == input) ? std::cout << "all good\n" : std::cout << "not good\n";
  }

  {
    std::vector<type> input(10 * 10 * 1);
    std::fill(input.begin(), input.end(), static_cast<type>(-7));
    std::cout << "test region: create, write, copy, read: ... ";
    auto gpu_input = cle::Array::create(10, 10, 1, cle::toType<type>(), mem_type, device);
    gpu_input->write(input.data());

    std::vector<type> subinput(5 * 5 * 1);
    std::fill(subinput.begin(), subinput.end(), static_cast<type>(1));

    gpu_input->write(subinput.data(), { 5, 5, 1 }, { 3, 1, 0 });
    std::vector<type> test(gpu_input->size());
    gpu_input->read(test.data());

    // for (int i = 0; i < test.size(); i++)
    // {
    //   if (i % 10 == 0)
    //     std::cout << std::endl;
    //   std::cout << test[i] << " ";
    // }
    // std::cout << std::endl;

    std::array<size_t, 3> region = { 6, 3, 1 };
    std::vector<type>     subtest(region[0] * region[1] * region[2]);
    gpu_input->read(subtest.data(), region, { 1, 1, 0 });

    // for (int i = 0; i < subtest.size(); i++)
    // {
    //   if (i % region[0] == 0)
    //     std::cout << std::endl;
    //   std::cout << subtest[i] << " ";
    // }
    // std::cout << std::endl;

    gpu_input->fill(0);
    gpu_input->read(input.data());

    // for (int i = 0; i < input.size(); i++)
    // {
    //   if (i % 10 == 0)
    //     std::cout << std::endl;
    //   std::cout << input[i] << " ";
    // }
    // std::cout << std::endl;
  }


  return 0;
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