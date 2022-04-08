
#include "clesperanto.hpp"

int main(int argc, char **argv)
{

    cle::Clesperanto cle;
    std::cout << "Devices available:\n";
    std::cout << cle.Ressources()->ListDevices() << std::endl << std::endl;
    std::cout << "Device info:\n";
    std::cout << cle.Ressources()->Info() << std::endl;

    return EXIT_SUCCESS;
}