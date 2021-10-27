
#include <random>
#include <iostream>
#include <vector>

#include "cleScalar.hpp"


/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    {
    cle::Scalar<int> x;
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<unsigned int> x(10);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<float> x(1.2f);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<double> x(1.2f);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<char> x(5);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<unsigned char> x(5);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<short> x(5);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<unsigned short> x(5);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    {
    cle::Scalar<> x(5);        
    std::cout << x.GetObjectType() << ": ("<< x.GetDataType() << ") " << x.Data();
    std::cout << " " << x.nDim() << "d ";
    std::cout << "[" << x.Shape()[0] << "," << x.Shape()[1] << ","<< x.Shape()[2] << "]" << std::endl;
    }

    return EXIT_SUCCESS;
}