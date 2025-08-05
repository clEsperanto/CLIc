#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMemory : public ::testing::TestWithParam<std::string>
{};



void my_function2(std::vector<cle::Array::Pointer> & src)
{
  // loop through the vector and print the count of the shared pointer array
  for (auto & array : src)
  {    
    std::cout << "\t\t" << array.use_count() << " -> " << array << std::endl;
  }
}


cle::Array::Pointer my_function(cle::Array::Pointer & src)
{
  std::vector<cle::Array::Pointer> arrays;
  for (int i = 0; i < 10; i++)
  {
    arrays.push_back(src);
  }

  my_function2(arrays);

  return src;
}


TEST_P(TestMemory, leak)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::cout << "start" << std::endl;

  // Create a new Array
  std::vector<float> data(10 * 20 * 1, 0.0f);

  std::cout << "create" << std::endl;
  auto array = cle::Array::create(10, 20, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  std::cout << "populate" << std::endl;
  array->writeFrom(data.data());
  std::cout << "multiply" << std::endl;

  // print the count of the shared pointer array
  // std::cout << array.use_count() << " -> " << array << std::endl;

  for (int i = 0; i < 100000; i++)
  {
    auto array2 = my_function(array);
  }

    std::cout << "bye bye" << std::endl;


  // std::cout << array.use_count() << " -> " << array << std::endl;
}



std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMemory, ::testing::ValuesIn(getParameters()));
