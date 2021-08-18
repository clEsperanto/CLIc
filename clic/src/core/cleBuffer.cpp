

#include "cleBuffer.hpp"
#include <iostream>
#include <algorithm>

namespace cle
{

Buffer::Buffer(cl::Buffer obj, unsigned int dimensions[3], LightObject::DataType type)
{
    this->m_Object = obj; 	
    for (size_t i = 0; i < 3; i++)
    {
        this->m_Dimensions[i] = dimensions[i];
    }    
    this->T = type;
}

Buffer::Buffer(cl::Buffer obj, LightObject::DataType type)
{
    this->m_Object = obj; 	
    this->T = type;
}

cl::Buffer Buffer::GetObject()
{
    return m_Object;
}

unsigned int* Buffer::GetDimensions()
{
    return this->m_Dimensions.data();
}

size_t Buffer::GetSize() const
{
    return m_Dimensions[0] * m_Dimensions[1] * m_Dimensions[2];
}

std::string Buffer::GetObjectType() const
{
    return this->ObjectTypeToString(O);
}

std::string Buffer::GetDataType() const
{
    return this->DataTypeToString(T);
}

bool Buffer::IsObjectType(LightObject::ObjectType str) const
{
    return this->O == str;
}

bool Buffer::IsDataType(LightObject::DataType str) const
{
    return this->T == str;
}

} // namespace cle

