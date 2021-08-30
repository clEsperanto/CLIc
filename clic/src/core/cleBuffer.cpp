#include "cleBuffer.hpp"

namespace cle
{

Buffer::Buffer(cl::Buffer _ocl, DataType _t) : m_Object(_ocl), m_Type(_t)
{
}

Buffer::Buffer(cl::Buffer _ocl, std::array<int,3> _dim, DataType _t) : m_Object(_ocl), m_Type(_t) 
{
    std::copy(_dim.begin(), _dim.end(), m_Shape.begin());
}

Buffer::Buffer(cl::Buffer _ocl, int _dim[3], DataType _t) : m_Object(_ocl), m_Type(_t) 
{
    std::copy(_dim, _dim+3, m_Shape.begin());
}


cl::Buffer Buffer::GetObject() const 
{ 
    return this->m_Object; 
}

int Buffer::GetWidth() const 
{ 
    return this->m_Shape[0]; 
}

int Buffer::GetHeight() const 
{ 
    return this->m_Shape[1]; 
}

int Buffer::GetDepth() const 
{ 
    return this->m_Shape[2]; 
}

int Buffer::GetSize() const 
{ 
    return this->m_Shape[0]*this->m_Shape[1]*this->m_Shape[2]; 
}

int Buffer::GetDimension() const 
{ 
    if (m_Shape[2] > 1) return 3;
    if (m_Shape[1] > 1) return 2;
    return  1;
}


std::array<int,3> Buffer::GetShape() const 
{
    return m_Shape; 
}

const char* Buffer::GetDataType() const 
{ 
    if (m_Type == DataType::FLOAT) return "float"; 
    if (m_Type == DataType::DOUBLE) return "double"; 
    if (m_Type == DataType::INT) return "int"; 
    if (m_Type == DataType::CHAR) return "char"; 
    if (m_Type == DataType::UINT) return "uint"; 
    if (m_Type == DataType::UCHAR) return "uchar"; 
    if (m_Type == DataType::SHORT) return "short"; 
    if (m_Type == DataType::USHORT) return "ushort"; 
    return "";
}

bool Buffer::IsDataType(const char* str) const
{
    size_t size = strlen(str);
    if (m_Type == DataType::FLOAT && strncmp("float", str, size) == 0) return true; 
    if (m_Type == DataType::DOUBLE && strncmp("double", str, size) == 0) return true; 
    if (m_Type == DataType::INT && strncmp("int", str, size) == 0) return true; 
    if (m_Type == DataType::CHAR && strncmp("char", str, size) == 0) return true;
    if (m_Type == DataType::UINT && strncmp("uint", str, size) == 0) return true; 
    if (m_Type == DataType::UCHAR && strncmp("uchar", str, size) == 0) return true; 
    if (m_Type == DataType::SHORT && strncmp("short", str, size) == 0) return true; 
    if (m_Type == DataType::USHORT && strncmp("ushort", str, size) == 0) return true; 
    return false;
}

const char* Buffer::GetObjectType() const 
{ 
    return "buffer"; 
}   

bool Buffer::IsObjectType(const char* str) const
{
    size_t size = strlen(str);
    return strncmp("buffer", str, size) == 0;
}

}