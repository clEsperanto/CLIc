
#include "CLE.hpp"

#include "cleBuffer.hpp"
#include "cleFloat.hpp"
#include "cleInt.hpp"

template<class T>
cle::Buffer buffer_class_constructor_test(cle::CLE clesperanto, cle::LightObject::DataType type, unsigned int dims[3])
{
    std::vector<T> data (dims[0]*dims[1]*dims[2]); 
    std::fill (data.begin(),data.end(), 128);
    cl::Buffer clBuffer = cle::CreateBuffer<T>(data.size(), clesperanto.GetGPU());
    cle::Buffer my_buffer1;    
    cle::Buffer my_buffer2(clBuffer, dims, type);
    cle::Buffer my_buffer3(clBuffer, type);
    return my_buffer2;
}

int buffer_class_method_test(cle::Buffer buff, cle::LightObject::DataType type, unsigned int dims[3])
{
    if (!buff.IsObjectType(cle::LightObject::cleBuffer))
    {
        return EXIT_FAILURE;
    }
    if (!buff.IsDataType(type))
    {
        return EXIT_FAILURE;
    }
    if (!buff.GetSize() != dims[0]*dims[1]*dims[2])
    {
        return EXIT_FAILURE;
    }
    if (!(buff.GetDimensions()[0] == dims[0] && buff.GetDimensions()[1] == dims[1] && buff.GetDimensions()[2] == dims[2]))
    {
        return EXIT_FAILURE;
    }
    std::cout << buff.GetObjectType() << "[" << buff.GetDataType() << "]" << std::endl;
    return EXIT_SUCCESS;
}

int buffer_object_test(cle::CLE clesperanto)
{
    unsigned int dims[3] = {10, 5, 3};   
    cle::Buffer buff_f = buffer_class_constructor_test<float>(clesperanto, cle::LightObject::Float, dims);
    if (!buffer_class_method_test(buff_f, cle::LightObject::Float, dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_i = buffer_class_constructor_test<int>(clesperanto, cle::LightObject::Int, dims);
    if (!buffer_class_method_test(buff_i, cle::LightObject::Int, dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_ui = buffer_class_constructor_test<unsigned int>(clesperanto, cle::LightObject::UInt, dims);
    if (!buffer_class_method_test(buff_ui, cle::LightObject::UInt, dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_c = buffer_class_constructor_test<char>(clesperanto, cle::LightObject::Char, dims);
    if (!buffer_class_method_test(buff_c, cle::LightObject::Char, dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_uc = buffer_class_constructor_test<unsigned char>(clesperanto, cle::LightObject::UChar, dims);
    if (!buffer_class_method_test(buff_uc, cle::LightObject::UChar, dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_s = buffer_class_constructor_test<short>(clesperanto, cle::LightObject::Short, dims);
    if (!buffer_class_method_test(buff_s, cle::LightObject::Short, dims))
    {
        return EXIT_FAILURE;
    }
    cle::Buffer buff_us = buffer_class_constructor_test<unsigned short>(clesperanto, cle::LightObject::UShort, dims);
    if (!buffer_class_method_test(buff_us, cle::LightObject::UShort, dims))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int float_object_test(float value)
{
    cle::Float my_float(value);
    if(my_float.GetObject() != value)
    {
        return EXIT_FAILURE;
    }
    if(my_float.GetSize() != 1)
    {
        return EXIT_FAILURE;
    }
    if(my_float.GetObjectType().compare("cleFloat") != 0)
    {
        return EXIT_FAILURE;
    }
    if(!my_float.IsObjectType(cle::LightObject::cleFloat))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int int_object_test(int value)
{
    cle::Int my_int(value);
    if(my_int.GetObject() != value)
    {
        return EXIT_FAILURE;
    }
    if(my_int.GetSize() != 1)
    {
        return EXIT_FAILURE;
    }
    if(my_int.GetObjectType().compare("cleInt") != 0)
    {
        return EXIT_FAILURE;
    }
    if(!my_int.IsObjectType(cle::LightObject::cleInt))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}





int main(int argc, char** argv)
{

    cle::GPU gpu;
    cle::CLE cle(gpu);

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