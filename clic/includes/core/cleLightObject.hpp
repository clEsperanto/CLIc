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
    int m_Dimension;

public: 
    LightObject() =default;
    virtual ~LightObject() = default;

    virtual int GetDimension() const =0;

    virtual int GetWidth() const =0;
    virtual int GetHeight() const =0;
    virtual int GetDepth() const =0;


    virtual const char* GetDataType() const =0;
    virtual bool IsDataType(const char*) const =0;    
    virtual const char* GetObjectType() const =0;
    virtual bool IsObjectType(const char*) const =0;
};

}

#endif //__cleLightObject_hpp