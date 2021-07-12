
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    cle::GPU gpu;
    gpu.GetInfo();

    gpu.SelectDevice("GTX");
    gpu.GetSelectedDeviceInfo();

    return EXIT_SUCCESS;
}