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

template<class T =float>
class Scalar : public LightObject
{
private:
    T m_obj = 0;

protected:
    const char* TemplateToString() const;

public: 
    Scalar() =default;
    Scalar(const T);
    ~Scalar() =default;

    const T Data() const;

    const int nDim() const;
    const int Size() const;
    const std::array<size_t,3> Shape() const;
    const std::array<size_t,3> Origin() const;
    const std::array<size_t,3> Region() const;

    const char* GetDataType() const;
    const bool IsDataType(const char*) const;
    const char* GetObjectType() const;  
    const bool IsObjectType(const char*) const;
};

template<class T>
Scalar<T>::Scalar(const T _d) : LightObject(), m_obj(_d)
{}

template<class T>
const T Scalar<T>::Data() const 
{ 
    return this->m_obj; 
}

template<class T>
const int Scalar<T>::nDim() const 
{ 
    return 1; 
}

template<class T>
const int Scalar<T>::Size() const 
{ 
    return 1; 
}

template<class T>
const std::array<size_t,3> Scalar<T>::Shape() const 
{ 
    return std::array<size_t,3>({1,1,1}); 
}

template<class T>
const std::array<size_t,3> Scalar<T>::Origin() const 
{ 
    return std::array<size_t,3>({0,0,0}); 
}

template<class T>
const std::array<size_t,3> Scalar<T>::Region() const 
{ 
    return std::array<size_t,3>({1,1,1}); 
}

template<class T>
const char* Scalar<T>::GetDataType() const 
{ 
    return TemplateToString(); 
}

template<class T>
const bool Scalar<T>::IsDataType(const char* t_str) const
{
    return strncmp(this->TemplateToString(), t_str, strlen(t_str)) == 0;
}

template<class T>
const char* Scalar<T>::GetObjectType() const 
{ 
    return "scalar"; 
}   

template<class T>
const bool Scalar<T>::IsObjectType(const char* t_str) const
{
    return strncmp("scalar", t_str, strlen(t_str)) == 0;
}

template<class T>
const char* Scalar<T>::TemplateToString() const
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