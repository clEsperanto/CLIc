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
public: 
    LightObject() =default;
    virtual ~LightObject() = default;

    virtual const char* GetDataType() const =0;
    virtual bool IsDataType(const char*) const =0;    
    virtual const char* GetObjectType() const =0;
    virtual bool IsObjectType(const char*) const =0;
};

}

#endif //__cleLightObject_hpp