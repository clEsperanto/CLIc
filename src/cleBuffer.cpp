/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include "cleBuffer.h"
#include <iostream>

namespace cle
{

// Buffer::Buffer()
// {
//     // pointer = nullptr;
//     // dimensions = {0, 0, 0};
//     // type = "";
//     // typeId = "";
// }  

Buffer::Buffer(cl_mem _ptr, unsigned int* _dimensions, std::string _type)
{
    pointer = _ptr;
    type = _type;
    typeId = this->TypeId(_type);
    int arrSize = sizeof(_dimensions)/sizeof(_dimensions[0]) +1;
    if (arrSize > 3)
    {
        arrSize = 3;
        std::cerr << "warning: Buffer maximum dimensions exeeded,"; 
        std::cerr << "only the three first values are considered.";
        std::cerr << std::endl;
    }
    for (size_t i = 0; i < arrSize; i++)
    {
        dimensions[i] = _dimensions[i];
    }    
}

Buffer::~Buffer()
{
}

std::array<unsigned int, 3> Buffer::GetDimensions()
{
    return dimensions;
}

std::string Buffer::GetType()
{
    return type;
}

std::string Buffer::GetTypeId()
{
    return typeId;
}

cl_mem Buffer::GetPointer()
{
    return pointer;
}

std::string Buffer::TypeId(std::string type)
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

std::string Buffer::to_str() const
{
    std::string typ = ", dtype=" + typeId + "(" + type + ")";
    std::string dim = "size=[" + std::to_string(dimensions[0]) + "," 
                               + std::to_string(dimensions[1]) + "," 
                               + std::to_string(dimensions[2]) + "]"; 
    return "clBuffer<" + dim + typ + ">";

}

std::ostream& operator<<(std::ostream& os, const Buffer& p)
{
    return os << p.to_str();
}

} // namespace cle

