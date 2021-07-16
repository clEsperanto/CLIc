
#include "CLE.h"

#include "cleBuffer.h"
#include "cleFloat.h"
#include "cleInt.h"

template<class T>
int buffer_class_constructor_test(cle::CLE clesperanto, cle::LightObject::DataType type)
{
    using data_type = T;
    unsigned int dims[3] = {10, 10, 10};   
    std::vector<data_type> data (dims[0]*dims[1]*dims[2]); 
    cl::Buffer clBuffer = cle::CreateBuffer<data_type>(data.size(), clesperanto.GetGPU());
    try 
    {
        cle::Buffer construtor1;    
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    try 
    {
        cle::Buffer construtor2(clBuffer, dims, type);
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    try 
    {
        cle::Buffer construtor3(clBuffer, type);
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

template<class T>
int buffer_class_method_test(cle::CLE clesperanto, cle::LightObject::DataType type)
{
    using data_type = T;
    unsigned int dims[3] = {10, 10, 10};   
    std::vector<data_type> data (dims[0]*dims[1]*dims[2]); 
    cl::Buffer clBuffer = cle::CreateBuffer<data_type>(data.size(), clesperanto.GetGPU());

    cle::Buffer my_buffer(clBuffer, dims, type);
    if(my_buffer.GetSize() != data.size())
    {
        return EXIT_FAILURE;
    }
    if(my_buffer.GetDimensions()[0] != dims[0] || my_buffer.GetDimensions()[1] != dims[1] || my_buffer.GetDimensions()[2] != dims[2])
    {
        return EXIT_FAILURE;
    }
    if(my_buffer.GetObjectType().compare("cleBuffer") != 0)
    {
        return EXIT_FAILURE;
    }
    if(!my_buffer.IsObject(cle::LightObject::cleBuffer))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int buffer_object_test(cle::CLE clesperanto)
{
    if(buffer_class_constructor_test<float>(clesperanto, cle::LightObject::Float))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_constructor_test<int>(clesperanto, cle::LightObject::Int))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_constructor_test<unsigned int>(clesperanto, cle::LightObject::UInt))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_constructor_test<char>(clesperanto, cle::LightObject::Char))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_constructor_test<unsigned char>(clesperanto, cle::LightObject::UChar))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_constructor_test<short>(clesperanto, cle::LightObject::Short))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_constructor_test<unsigned short>(clesperanto, cle::LightObject::UShort))
    {
        return EXIT_FAILURE;
    }

    if(buffer_class_method_test<float>(clesperanto, cle::LightObject::Float))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_method_test<int>(clesperanto, cle::LightObject::Int))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_method_test<unsigned int>(clesperanto, cle::LightObject::UInt))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_method_test<char>(clesperanto, cle::LightObject::Char))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_method_test<unsigned char>(clesperanto, cle::LightObject::UChar))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_method_test<short>(clesperanto, cle::LightObject::Short))
    {
        return EXIT_FAILURE;
    }
    if(buffer_class_method_test<unsigned short>(clesperanto, cle::LightObject::UShort))
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
    if(!my_float.IsObject(cle::LightObject::cleFloat))
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
    if(!my_int.IsObject(cle::LightObject::cleInt))
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