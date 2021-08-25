
#include "clesperanto.hpp"

#include "cleBuffer.hpp" 


template<class T>
int buffer_test(cle::Clesperanto* cle, cle::Buffer::DataType type, const char * c_type)
{
    int dims[3] = {5, 4, 3};
    std::vector<T> arr (dims[0]*dims[1]*dims[2]); 
    std::fill (arr.begin(),arr.end(), 1);

    cl::Buffer clObject = cle::CreateBuffer<T>(arr.size(), cle->GetGPU());
    int res = 0;

    {
    cle::Buffer* my_buffer = new cle::Buffer(); 
    delete my_buffer;   
    }

    {
    cle::Buffer* my_buffer = new cle::Buffer(clObject, type);
    delete my_buffer;
    }

    {
    cle::Buffer* my_buffer = new cle::Buffer(clObject, dims, type);
    res += my_buffer->IsObjectType("buffer") ;
    res += my_buffer->IsObjectType("scalar") != 0;
    res += my_buffer->IsDataType(c_type) ;
    res += my_buffer->IsDataType("toto") != 0 ;
    res += my_buffer->GetSize() == (dims[0]*dims[1]*dims[2]) ;
    res += my_buffer->GetShape().data()[0] == dims[0] ;
    res += my_buffer->GetShape()[1] == dims[1] ;
    res += my_buffer->GetShape()[2] == dims[2] ;
    res += my_buffer->GetWidth() == dims[0] ;
    res += my_buffer->GetHeight() == dims[1] ;
    res += my_buffer->GetDepth() == dims[2] ;
    res += strncmp(my_buffer->GetObjectType(), "buffer", strlen("buffer")) == 0 ;
    res += strncmp(my_buffer->GetObjectType(), "scalar", strlen("scalar")) != 0 ;
    res += strncmp(my_buffer->GetDataType(), c_type, strlen(c_type)) == 0 ;
    res += strncmp(my_buffer->GetDataType(), "toto", strlen("toto")) != 0 ;
    delete my_buffer;
    }

    return res;
}



int main(int argc, char** argv)
{
    cle::Clesperanto* cle = new cle::Clesperanto();

    int res = 0;
    res += buffer_test<float>(cle, cle::Buffer::FLOAT, "float");
    res += buffer_test<double>(cle, cle::Buffer::DOUBLE, "double");
    res += buffer_test<int>(cle, cle::Buffer::INT, "int");
    res += buffer_test<unsigned int>(cle, cle::Buffer::UINT, "uint");
    res += buffer_test<char>(cle, cle::Buffer::CHAR, "char");
    res += buffer_test<unsigned char>(cle, cle::Buffer::UCHAR, "uchar");
    res += buffer_test<short>(cle, cle::Buffer::SHORT, "short");
    res += buffer_test<unsigned short>(cle, cle::Buffer::USHORT, "ushort");

    return res == 0;
}