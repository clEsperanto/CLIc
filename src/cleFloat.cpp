/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/



#include "cleFloat.h"

#include <string>

namespace cle
{

Float::Float(float v)
{
    value = v;
}

float& Float::GetData()
{
    return value;
}

std::string Float::GetObjectType() const
{
    return this->ObjectTypeToString(O);
}

std::string Float::ToString() const
{
    std::string str = this->GetObjectType() + "(float) = " + std::to_string(value);
    return str;
}

bool Float::IsObject(std::string str) const
{
    return this->GetObjectType() == str;
}

size_t Float::GetBitSize() const
{
    return sizeof(float);
}

} // namespace cle

