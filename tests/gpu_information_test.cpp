
#include "clesperanto.hpp"

int main(int argc, char **argv)
{

    cle::Clesperanto cle;
    std::cout << cle.Ressources()->Info() << std::endl;

    return EXIT_SUCCESS;
}