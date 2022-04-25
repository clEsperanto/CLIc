
#include "clesperanto.hpp"

int main(int argc, char **argv)
{

    cle::Clesperanto cle;
    std::cout << "Devices available:\n";
    auto device_list = cle.Ressources()->ListAvailableDevices();
    std::cout << "[";
    for (auto it = device_list.begin(); it != device_list.end(); ++it)
    {
        std::cout << *it;
        if ((it+1) != device_list.end())
        {
            std::cout << " ; ";
        } 
    }
    std::cout << "]\n";
    std::cout << "Device info:\n";
    std::cout << cle.Ressources()->Info() << std::endl;

    return EXIT_SUCCESS;
}