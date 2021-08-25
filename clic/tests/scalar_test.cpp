

#include "cleScalar.hpp" 


template<class T>
int scalar_test(const char* c_type)
{
    T value = 5;
    int res = 0;

    {
    cle::Scalar<T>* my_scalar = new cle::Scalar<T>(); 
    delete my_scalar;   
    }

    {
    cle::Scalar<T>* my_scalar = new cle::Scalar<T>(value); 
    res += my_scalar->IsObjectType("buffer") != 0;
    res += my_scalar->IsObjectType("scalar");
    res += my_scalar->IsDataType(c_type) ;
    res += my_scalar->IsDataType("toto") != 0 ;
    res += strncmp(my_scalar->GetObjectType(), "buffer", strlen("buffer")) != 0 ;
    res += strncmp(my_scalar->GetObjectType(), "scalar", strlen("scalar")) == 0 ;
    res += !strncmp(my_scalar->GetDataType(), c_type, strlen(c_type)) == 0 ;
    res += strncmp(my_scalar->GetDataType(), "toto", strlen("toto")) != 0 ;
    res += my_scalar->GetObject() == value;
    delete my_scalar;
    }

    return res;
}



int main(int argc, char** argv)
{
    int res = 0;
    res += scalar_test<float>("float");
    res += scalar_test<double>("double");
    res += scalar_test<int>("int");
    res += scalar_test<unsigned int>("uint");
    res += scalar_test<char>("char");
    res += scalar_test<unsigned char>("uchar");
    res += scalar_test<short>("short");
    res += scalar_test<unsigned short>("ushort");

    return res == 0;
}