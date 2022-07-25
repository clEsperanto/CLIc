
#include "cleScalar.hpp"

#include <cassert>
#include <iostream>

template <class type>
auto
test_scalar (const type &x) -> bool
{
    cle::Scalar<type> scalar (x);
    std::cout << scalar.ToString () << std::endl;
    return (x == scalar.Get ());
}

/**
 * Main test function
 *
 */
auto
main (int argc, char **argv) -> int
{
    double input = std::atof (argv[1]);
    {
        assert (test_scalar<int> (static_cast<int> (input)));
    }

    {
        assert (test_scalar<float> (static_cast<float> (input)));
    }

    return EXIT_SUCCESS;
}