#ifndef __cleScalar_hpp
#define __cleScalar_hpp

#include "clic.hpp"
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>

#include "cleLightObject.hpp"

namespace cle
{

template<class T>
class Scalar : public LightObject
{
private:
    T m_Object;

protected:
    const char * TemplateToString() const;
public: 

    Scalar();
    Scalar(T);
    ~Scalar() =default;

    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetDimension() const;

    T GetObject() const;

    const char* GetDataType() const;

    bool IsDataType(const char* str) const;
    const char* GetObjectType() const;  

    bool IsObjectType(const char* str) const;
    
};

template<class T>
Scalar<T>::Scalar() : m_Object(0) 
{
}

template<class T>
Scalar<T>::Scalar(T _d) : m_Object(_d)
{
}

template<class T>
int Scalar<T>::GetWidth() const
{
    return 1;
}

template<class T>
int Scalar<T>::GetHeight() const
{
    return 1;
}

template<class T>
int Scalar<T>::GetDepth() const
{
    return 1;
}

template<class T>
int Scalar<T>::GetDimension() const
{
    return 1;
}

template<class T>
T Scalar<T>::GetObject() const { return this->m_Object; }

template<class T>
const char* Scalar<T>::GetDataType() const { return TemplateToString(); }

template<class T>
bool Scalar<T>::IsDataType(const char* str) const
{
    size_t size = strlen(str);
    return strncmp(TemplateToString(), str, size) == 0;
}

template<class T>
const char* Scalar<T>::GetObjectType() const { return "scalar"; }   

template<class T>
bool Scalar<T>::IsObjectType(const char* str) const
{
    size_t size = strlen(str);
    return strncmp("scalar", str, size) == 0;
}

template<class T>
const char * Scalar<T>::TemplateToString() const
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


}

#endif //__cleScalar_hpp