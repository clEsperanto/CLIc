
#include "clesperanto.hpp"

#include "cleScalar.hpp"
#include "cleBuffer.hpp" 
 

template<class T>
cle::Buffer buffer_class_constructor_test(cle::Clesperanto clesperanto, cle::Buffer::DataType type, int dims[3])
{
    std::vector<T> data (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 128);
    cl::Buffer clBuffer = cle::CreateBuffer<T>(data.size(), clesperanto.GetGPU());
    cle::Buffer my_buffer1;    
    cle::Buffer my_buffer2(clBuffer, dims, type);
    cle::Buffer my_buffer3(clBuffer, type);
    return my_buffer2;
}

int buffer_class_method_test(cle::Buffer buff, const char * type, int dims[3])
{
    if (!buff.IsObjectType("buffer"))
    {
        return EXIT_FAILURE;
    }
    if (!buff.IsDataType(type))
    {
        return EXIT_FAILURE;
    }
    if (!(buff.GetSize() != dims[0]*dims[1]*dims[2]))
    {
        return EXIT_FAILURE;
    }
    if (!(buff.GetShape()[0] == dims[0] && buff.GetShape()[1] == dims[1] && buff.GetShape()[2] == dims[2]))
    {
        return EXIT_FAILURE;
    }
    std::cout << buff.GetObjectType() << "[" << buff.GetDataType() << "]" << std::endl;
    return EXIT_SUCCESS;
}

int buffer_object_test(cle::Clesperanto clesperanto)
{
    int dims[3] = {10, 5, 3};   
    cle::Buffer buff_f = buffer_class_constructor_test<float>(clesperanto, cle::Buffer::FLOAT, dims);
    if (!buffer_class_method_test(buff_f, "float", dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_i = buffer_class_constructor_test<int>(clesperanto, cle::Buffer::INT, dims);
    if (!buffer_class_method_test(buff_i, "int", dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_ui = buffer_class_constructor_test<unsigned int>(clesperanto, cle::Buffer::UINT, dims);
    if (!buffer_class_method_test(buff_ui, "unsigned int", dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_c = buffer_class_constructor_test<char>(clesperanto, cle::Buffer::CHAR, dims);
    if (!buffer_class_method_test(buff_c, "char", dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_uc = buffer_class_constructor_test<unsigned char>(clesperanto, cle::Buffer::UCHAR, dims);
    if (!buffer_class_method_test(buff_uc, "unsigned char", dims))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int float_object_test(float value)
{
    cle::Scalar<float> my_float(value);
    if(my_float.GetObject() != value)
    {
        return EXIT_FAILURE;
    }
    if(strncmp(my_float.GetObjectType(), "scalar", strlen("scalar")) != 0)
    {
        return EXIT_FAILURE;
    }
    if(!my_float.IsObjectType("scalar"))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int int_object_test(int value)
{
    cle::Scalar<int> my_int(value);
    if(my_int.GetObject() != value)
    {
        return EXIT_FAILURE;
    }
    if(strncmp(my_int.GetObjectType(), "scalar", strlen("scalar")) != 0)
    {
        return EXIT_FAILURE;
    }
    if(!my_int.IsObjectType("scalar"))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}





int main(int argc, char** argv)
{

    cle::Clesperanto cle;

    if(buffer_object_test(cle))
    {
        return EXIT_FAILURE;
    }

    if(float_object_test(0.5))
    {
        return EXIT_FAILURE;
    }

    if(int_object_test(128))
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}