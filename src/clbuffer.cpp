/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include "clbuffer.h"
#include <iostream>

clBuffer::clBuffer(cl_mem _ptr, unsigned int* _dimensions, std::string _type)
{
    pointer = _ptr;
    type = _type;
    typeId = this->TypeId(_type);
    int arrSize = sizeof(_dimensions)/sizeof(_dimensions[0]) +1;
    if (arrSize > 3)
    {
        arrSize = 3;
        std::cerr << "warning: clBuffer maximum dimensions exeeded,"; 
        std::cerr << "only the three first values are considered.";
        std::cerr << std::endl;
    }
    for (size_t i = 0; i < arrSize; i++)
    {
        dimensions[i] = _dimensions[i];
    }    
}

clBuffer::~clBuffer()
{
}

std::array<unsigned int, 3> clBuffer::GetDimensions()
{
    return dimensions;
}

std::string clBuffer::GetType()
{
    return type;
}

std::string clBuffer::GetTypeId()
{
    return typeId;
}

cl_mem clBuffer::GetPointer()
{
    return pointer;
}

std::string clBuffer::TypeId(std::string type)
{
    std::string res;
    if (type.compare("float") == 0)
    {
        res = "f";
    }
    else if (type.compare("char") == 0)
    {
        res =  "c";
    }
    else if (type.compare("uchar") == 0)
    {
        res =  "uc";
    }
    else if (type.compare("int") == 0)
    {
        res =  "i";
    }
    else if (type.compare("uint") == 0)
    {
        res =  "ui";
    }
    else
    {
        res = "f";
    }
    return res; 
}
