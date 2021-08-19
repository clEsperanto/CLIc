#ifndef __datatype_hpp
#define __datatype_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

namespace cle
{

class LightObject
{
public: 
    LightObject() =default;
    virtual ~LightObject() = default;

    virtual const char* GetDataType() const =0;
    virtual bool IsDataType(const char*) const =0;    
    virtual const char* GetObjectType() const =0;
    virtual bool IsObjectType(const char*) const =0;
};

template<class T>
class Scalar : public LightObject
{
private:
    T m_Object;

protected:
    const char * TemplateToString() const
    {
        if(std::is_same<T, float>::value)  return "float";
        if(std::is_same<T, int>::value) return "int";
        if(std::is_same<T, unsigned int>::value) return "uint";
        if(std::is_same<T, char>::value) return "char";
        if(std::is_same<T, unsigned char>::value) return "uchar";
        if(std::is_same<T, double>::value) return "double";
        if(std::is_same<T, short>::value) return "short";
        if(std::is_same<T, unsigned short>::value) return "ushort";
        return "unknown";
    } 

public: 

    Scalar() =default;
    Scalar(T _d) : m_Object(_d) {}
    ~Scalar() =default;

    T GetObject() const { return this->m_Object; }

    const char* GetDataType() const { return TemplateToString(); }

    bool IsDataType(const char* str) const
    {
        size_t size = strlen(str);
        return strncmp(TemplateToString(), str, size) == 0;
    }

    const char* GetObjectType() const { return "scalar"; }   

    bool IsObjectType(const char* str) const
    {
        size_t size = strlen(str);
        return strncmp("scalar", str, size) == 0;
    }
    
};


class Buffer : public LightObject
{
public: 

    enum DataType { FLOAT, DOUBLE, INT, UINT, CHAR, UCHAR, SHORT, USHORT };

    Buffer() =default;
    Buffer(cl::Buffer, DataType=FLOAT);
    Buffer(cl::Buffer, std::array<int,3>, DataType=FLOAT);
    Buffer(cl::Buffer, int[3], DataType=FLOAT);
    ~Buffer() =default;

    cl::Buffer GetObject() const;
    
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetDimension() const;
    int GetSize() const;
    std::array<int,3> GetShape() const;

    const char* GetDataType() const;
    bool IsDataType(const char*) const;
    const char* GetObjectType() const;
    bool IsObjectType(const char*) const;

private:
    cl::Buffer m_Object;
    std::array<int,3> m_Shape = {1, 1, 1};
    int m_Dimension = 1;
    DataType m_Type = Buffer::FLOAT;
};

}

#endif //__datatype_hpp