#ifndef __cleObject_hpp
#define __cleObject_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleLightObject.hpp"

namespace cle
{

class Object : public LightObject
{
public: 

    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    Object() = default;
    Object(const std::array<int,3>& t_shape, const DataType t_dtype): LightObject(t_shape), m_dType(t_dtype)
    {};    
    ~Object() =default;

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
    }

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
    }

    const char* GetObjectType() const =0;
    const bool IsObjectType(const char*) const =0;

protected:
    DataType m_dType;
};

}

#endif //__cleObject_hpp