#ifndef __cleLightObject_hpp
#define __cleLightObject_hpp

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
protected:
    int m_ndim =1;
    std::array<int,3> m_Shape = {1, 1, 1};

public: 
    LightObject() =default;
    LightObject(const std::array<int,3>& t_shape) : m_Shape(t_shape)
    {
        if (this->m_Shape[2]>1) this->m_ndim = 3;
        else if (this->m_Shape[1]>1) this->m_ndim = 2;
        else this->m_ndim = 1;
    }
    virtual ~LightObject() =default;

    const int nDim() const { return m_ndim; };
    const std::array<int,3> Shape() const { return m_Shape; };
    const int Size() const { return this->m_Shape[0]*this->m_Shape[1]*this->m_Shape[2]; };

    virtual const char* GetDataType() const =0;
    virtual const bool IsDataType(const char*) const =0;    
    virtual const char* GetObjectType() const =0;
    virtual const bool IsObjectType(const char*) const =0;
};

}

#endif //__cleLightObject_hpp