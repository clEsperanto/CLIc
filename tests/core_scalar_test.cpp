
#include "cleScalar.hpp"

#include <iostream>

template <class type>
auto
run_test(const type & x) -> bool
{
  cle::Scalar<type> scalar(x);
  std::cout << scalar << std::endl;
  return (x == scalar.Get());
}

auto
main(int argc, char ** argv) -> int
{

  if (!run_test<float>(static_cast<float>(1.5F)))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>(static_cast<int>(2)))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}