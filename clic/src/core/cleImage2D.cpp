#include "cleImage2D.hpp"

namespace cle
{
Image2D::Image2D()
{}

Image2D::Image2D(cl::Image2D _ocl, DataType _t) : m_Object(_ocl), m_Type(_t)
{}

Image2D::Image2D(cl::Image2D _ocl, std::array<int,3> _dim, DataType _t) : m_Object(_ocl), m_Type(_t)
{
    std::copy(_dim.begin(), _dim.end(), m_Shape.begin());
    std::copy(_dim.begin(), _dim.end(), m_Region.begin());
    m_ImageFormat.image_channel_order = _ocl.getImageInfo<CL_IMAGE_FORMAT>().image_channel_order;          
    m_ImageFormat.image_channel_data_type = _ocl.getImageInfo<CL_IMAGE_FORMAT>().image_channel_data_type;  
}

Image2D::Image2D(cl::Image2D _ocl, int _dim[3] , DataType _t) : m_Object(_ocl), m_Type(_t)
{
    std::copy(_dim, _dim+3, m_Shape.begin());
    std::copy(_dim, _dim+3, m_Region.begin());
    m_ImageFormat.image_channel_order = _ocl.getImageInfo<CL_IMAGE_FORMAT>().image_channel_order;          
    m_ImageFormat.image_channel_data_type = _ocl.getImageInfo<CL_IMAGE_FORMAT>().image_channel_data_type;  
}

const cl::Image2D Image2D::GetObject() const
{
    return this->m_Object; 
}

int Image2D::GetWidth() const
{
    return m_Shape[0];
}

int Image2D::GetHeight() const
{
    return m_Shape[1];
}

int Image2D::GetDepth() const
{
    return m_Shape[2];
}

int Image2D::GetSize() const
{
    return m_Shape[0]*m_Shape[1]*m_Shape[2];
}

int Image2D::GetDimension() const
{
    return 2;
}

std::array<int,3> Image2D::GetShape() const
{
    return m_Shape;
}

std::array<int,3> Image2D::GetOrigin() const
{
    return m_Origin;
}

std::array<int,3> Image2D::GetRegion() const
{
    return m_Region;
}

const char* Image2D::GetDataType() const
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

bool Image2D::IsDataType(const char* str) const
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

const char* Image2D::GetObjectType() const
{
    return "image";
}

bool Image2D::IsObjectType(const char* str) const
{
    return strncmp("image", str, strlen(str)) == 0;
}

}