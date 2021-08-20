#include "cleBuffer.hpp"

namespace cle
{

Buffer::Buffer(cl::Buffer _ocl, DataType _t) : m_Object(_ocl), m_Type(_t) 
{}

Buffer::Buffer(cl::Buffer _ocl, std::array<int,3> _dim, DataType _t) : m_Object(_ocl), m_Type(_t) 
{
    std::copy(_dim.begin(), _dim.end(), m_Shape.begin());
    if (m_Shape[2] > 1) m_Dimension = 3;
    else if (m_Shape[1] > 1) m_Dimension = 2;
}

Buffer::Buffer(cl::Buffer _ocl, int _dim[3], DataType _t) : m_Object(_ocl), m_Type(_t) 
{
    std::copy(_dim, _dim+3, m_Shape.begin());
    if (m_Shape[2] > 1) m_Dimension = 3;
    else if (m_Shape[1] > 1) m_Dimension = 2;
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

int Buffer::GetDimension() const 
{ 
    return this->m_Dimension; 
}

int Buffer::GetSize() const 
{ 
    return this->m_Shape[0]*this->m_Shape[1]*this->m_Shape[2]; 
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
    if (strncmp("float", str, size) == 0 && m_Type == DataType::FLOAT) return true; 
    if (strncmp("double", str, size) == 0 && m_Type == DataType::DOUBLE) return true; 
    if (strncmp("int", str, size) == 0 && m_Type == DataType::INT) return true; 
    if (strncmp("char", str, size) == 0 && m_Type == DataType::CHAR) return true;
    if (strncmp("uint", str, size) == 0 && m_Type == DataType::UINT) return true; 
    if (strncmp("uchar", str, size) == 0 && m_Type == DataType::UCHAR) return true; 
    if (strncmp("short", str, size) == 0 && m_Type == DataType::SHORT) return true; 
    if (strncmp("ushort", str, size) == 0 && m_Type == DataType::USHORT) return true; 
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