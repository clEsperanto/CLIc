/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleBuffer.h"
#include <iostream>
#include <algorithm>

namespace cle
{

Buffer::Buffer(cl_mem _ptr, unsigned int* _dimensions, std::string _type)
{
    pointer = _ptr;
    T = this->StringToDataType(_type);
    int arrSize = sizeof(_dimensions)/sizeof(_dimensions[0]) +1;
    if (arrSize > 3)
    {
        std::cerr << "Warning: 3 Dimensions maximum, "; 
        std::cerr << "additional dimensions are ignored.";
        std::cerr << std::endl;
    }
    for (size_t i = 0; i < std::min(arrSize, 3); i++)
    {
        dimensions[i] = _dimensions[i];
    }    
}

unsigned int* Buffer::GetDimensions()
{
    return dimensions.data();
}

cl_mem& Buffer::GetData()
{
    return pointer;
}

std::string Buffer::GetObjectType() const
{
    return this->ObjectTypeToString(O);
}

std::string Buffer::GetDataType() const
{
    return this->DataTypeToString(T);
}

std::string Buffer::ToString() const
{
    std::string str = "";
    str += this->GetObjectType() + "(" + this->GetDataType() + ")";
    str += " [" + std::to_string(this->dimensions[0]) ;
    str += ","  + std::to_string(this->dimensions[1]) ;
    str += ","  + std::to_string(this->dimensions[2]) + "]";
    return str;
}

bool Buffer::IsObject(std::string str) const
{
    return this->GetObjectType() == str;
}

size_t Buffer::GetBitSize() const
{
    switch (T)
    {
        case Float:  return sizeof(float);
        case Char:   return sizeof(char);
        case UChar:  return sizeof(unsigned char);
        case Int:    return sizeof(int);
        case UInt:   return sizeof(unsigned int);
        case Short:  return sizeof(short);
        case UShort: return sizeof(unsigned short);
        default:     return 0;
    }
}
} // namespace cle

