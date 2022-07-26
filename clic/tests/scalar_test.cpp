
#include "cleScalar.hpp"

#include <cassert>
#include <iostream>

template <class type>
auto
run_test (const type &x) -> bool
{
    cle::Scalar<type> scalar (x);
    std::cout << scalar.ToString () << std::endl;
    return (x == scalar.Get ());
}

auto
main (int argc, char **argv) -> int
{
    int idx = 0;
    std::string data_t = argv[++idx];
    double input = std::atof (argv[++idx]);
    if (data_t == "float")
        {
            assert (run_test<float> (static_cast<float> (input)));
        }
    if (data_t == "int")
        {
            assert (run_test<int> (static_cast<int> (input)));
        }
    return EXIT_SUCCESS;
}