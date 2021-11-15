#ifndef __cleObject_hpp
#define __cleObject_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>
#include <functional>
#include <numeric>

#include "cleLightObject.hpp"

namespace cle
{

class Object : public LightObject
{
public: 

    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    Object() = default;
    Object(const std::array<size_t,3>& t_shape, const DataType t_dtype, const char* t_otype): LightObject(), m_dType(t_dtype), m_oType(t_otype), m_Shape(t_shape), m_Region(t_shape)
    {
        if (this->m_Shape[2]>1) this->m_ndim = 3;
        else if (this->m_Shape[1]>1) this->m_ndim = 2;
        else this->m_ndim = 1;
    };    
    ~Object() =default;

    const int nDim() const { return m_ndim; };
    const int Size() const { return static_cast<int>(std::accumulate(m_Shape.begin(), m_Shape.end(), 1, std::multiplies<size_t>())); };
    const std::array<size_t,3> Shape() const { return m_Shape; };
    const std::array<size_t,3> Origin() const { return m_Origin; };
    const std::array<size_t,3> Region() const { return m_Region; };
    const DataType Type() const { return m_dType; };

    const char* GetDataType() const 
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

    const bool IsDataType(const char* t_str) const
    {
        size_t size = strlen(t_str);
        if (this->m_dType == DataType::FLOAT && strncmp("float", t_str, size) == 0) return true; 
        if (this->m_dType == DataType::DOUBLE && strncmp("double", t_str, size) == 0) return true; 
        if (this->m_dType == DataType::INT && strncmp("int", t_str, size) == 0) return true; 
        if (this->m_dType == DataType::CHAR && strncmp("char", t_str, size) == 0) return true;
        if (this->m_dType == DataType::UINT && strncmp("uint", t_str, size) == 0) return true; 
        if (this->m_dType == DataType::UCHAR && strncmp("uchar", t_str, size) == 0) return true; 
        if (this->m_dType == DataType::SHORT && strncmp("short", t_str, size) == 0) return true; 
        if (this->m_dType == DataType::USHORT && strncmp("ushort", t_str, size) == 0) return true; 
        return false;
    };

    const char* GetObjectType() const { return m_oType; };

    const bool IsObjectType(const char* t_str) const { return strncmp(this->m_oType, t_str, strlen(t_str)) == 0; };

protected:

    // std::shared_ptr<const cl::Memory> m_Ocl =nullptr;
    DataType m_dType = DataType::FLOAT;
    const char* m_oType = "object";
    int m_ndim = 1;
    std::array<size_t,3> m_Shape {1, 1, 1};
    std::array<size_t,3> m_Origin {0, 0, 0};
    std::array<size_t,3> m_Region {1, 1, 1};
};

}

#endif //__cleObject_hpp