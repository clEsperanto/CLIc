
#include "cleObject.hpp"

#include <numeric>

namespace cle
{

Object::Object(const cl::Memory& t_object, const std::array<size_t,3>& t_shape, const DataType t_dtype): LightObject(), m_Ocl(t_object), m_dType(t_dtype), m_Shape(t_shape), m_Region(t_shape)
{
    switch (this->m_Ocl.getInfo<CL_MEM_TYPE>())
    {
        case CL_MEM_OBJECT_BUFFER: 
            {
            this->m_oType =  "buffer"; break;
            }
        case CL_MEM_OBJECT_IMAGE1D: 
            {
            this->m_oType = "image1d"; 
            this->m_ndim = 1; break;
            }
        case CL_MEM_OBJECT_IMAGE2D: 
            {
            this->m_oType = "image2d"; 
            this->m_ndim = 2; break;
            }
        case CL_MEM_OBJECT_IMAGE3D: 
            {
            this->m_oType = "image3d"; 
            this->m_ndim = 3; break;
            }
        default: 
            {
            this->m_oType = "unknown";
            }
    }
    if (this->m_ndim == 0)
    {
        if (this->m_Shape[2]>1) this->m_ndim = 3;
        else if (this->m_Shape[1]>1) this->m_ndim = 2;
        else this->m_ndim = 1;
    }
};

const cl::Memory& Object::Data() const
{
    return this->m_Ocl;
}

const cl_mem_object_type Object::MemoryType() const
{
    return this->m_Ocl.getInfo<CL_MEM_TYPE>();
};

const cl::size_type Object::MemorySize() const
{
    return this->m_Ocl.getInfo<CL_MEM_SIZE>();
};

const int Object::nDim() const 
{ 
    return m_ndim; 
};

const int Object::Size() const 
{ 
    return static_cast<int>(std::accumulate(m_Shape.begin(), m_Shape.end(), 1, std::multiplies<size_t>())); 
};

const std::array<size_t,3> Object::Shape() const 
{ 
    return m_Shape; 
};

const std::array<size_t,3> Object::Origin() const 
{ 
    return m_Origin; 
};

const std::array<size_t,3> Object::Region() const 
{ 
    return m_Region; 
};

const Object::DataType Object::Type() const 
{ 
    return m_dType; 
};

const char* Object::GetDataType() const 
{ 
    if (this->m_dType == cle::Object::DataType::FLOAT) return "float"; 
    if (this->m_dType == cle::Object::DataType::DOUBLE) return "double"; 
    if (this->m_dType == cle::Object::DataType::INT) return "int"; 
    if (this->m_dType == cle::Object::DataType::CHAR) return "char"; 
    if (this->m_dType == cle::Object::DataType::UINT) return "uint"; 
    if (this->m_dType == cle::Object::DataType::UCHAR) return "uchar"; 
    if (this->m_dType == cle::Object::DataType::SHORT) return "short"; 
    if (this->m_dType == cle::Object::DataType::USHORT) return "ushort"; 
    return "";
};

const bool Object::IsDataType(const char* t_dtype) const
{
    size_t size = strlen(t_dtype);
    if (this->m_dType == DataType::FLOAT && strncmp("float", t_dtype, size) == 0) return true; 
    if (this->m_dType == DataType::DOUBLE && strncmp("double", t_dtype, size) == 0) return true; 
    if (this->m_dType == DataType::INT && strncmp("int", t_dtype, size) == 0) return true; 
    if (this->m_dType == DataType::CHAR && strncmp("char", t_dtype, size) == 0) return true;
    if (this->m_dType == DataType::UINT && strncmp("uint", t_dtype, size) == 0) return true; 
    if (this->m_dType == DataType::UCHAR && strncmp("uchar", t_dtype, size) == 0) return true; 
    if (this->m_dType == DataType::SHORT && strncmp("short", t_dtype, size) == 0) return true; 
    if (this->m_dType == DataType::USHORT && strncmp("ushort", t_dtype, size) == 0) return true; 
    return false;
};

const char* Object::GetObjectType() const 
{ 
    return m_oType; 
};

const bool Object::IsObjectType(const char* t_otype) const 
{ 
    return strncmp(this->m_oType, t_otype, strlen(t_otype)) == 0; 
};

const bool Object::IsMemoryType(const cl_mem_object_type t_type) const 
{ 
    return m_Ocl.getInfo<CL_MEM_TYPE>() == t_type; 
};

std::ostream& operator<<(std::ostream& os, const Object& obj)
{
    os << std::string("cle::") << obj.GetObjectType() << "(shape=[" << obj.Shape()[0]<<","<< obj.Shape()[1]<<","<< obj.Shape()[2] << "],type=\'" << obj.GetDataType()<<"\')";
    return os;
}

} // namespace cle
