/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleScalar.h"
#include <iostream>

namespace cle
{

Scalar::Scalar(float v)
{
    value = v;
}

Scalar::~Scalar()
{
}

float Scalar::GetValue()
{
    return value;
}

std::string Scalar::GetObjectType() const
{
    return this->ObjectTypeToString(O);
}

std::string Scalar::GetDataType() const
{
    return this->DataTypeToString(T);
}

std::string Scalar::ToString() const
{
    // std::string typ = ", dtype=" + typeId + "(" + type + ")";
    // std::string dim = "size=[" + std::to_string(dimensions[0]) + "," 
    //                            + std::to_string(dimensions[1]) + "," 
    //                            + std::to_string(dimensions[2]) + "]"; 
    // return "clScalar<" + dim + typ + ">";
    return "";
}

} // namespace cle

