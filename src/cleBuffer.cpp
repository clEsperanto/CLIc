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

Buffer::~Buffer()
{
}

unsigned int* Buffer::GetDimensions()
{
    return dimensions.data();
}

cl_mem& Buffer::GetData()
{
    return pointer;
}

// std::string Buffer::TypeId(std::string type)
// {
//     std::string res;
//     if (type.compare("float") == 0)
//     {
//         res = "f";
//     }
//     else if (type.compare("char") == 0)
//     {
//         res =  "c";
//     }
//     else if (type.compare("uchar") == 0)
//     {
//         res =  "uc";
//     }
//     else if (type.compare("int") == 0)
//     {
//         res =  "i";
//     }
//     else if (type.compare("uint") == 0)
//     {
//         res =  "ui";
//     }
//     else
//     {
//         res = "f";
//     }
//     return res; 
// }

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
    // std::string typ = ", dtype=" + typeId + "(" + type + ")";
    // std::string dim = "size=[" + std::to_string(dimensions[0]) + "," 
    //                            + std::to_string(dimensions[1]) + "," 
    //                            + std::to_string(dimensions[2]) + "]"; 
    // return "clBuffer<" + dim + typ + ">";
    return "";
}

} // namespace cle

