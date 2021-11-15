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
    virtual ~LightObject() =default;

    virtual const int nDim() const =0;
    virtual const int Size() const =0;
    virtual const std::array<size_t,3> Shape() const =0;
    virtual const std::array<size_t,3> Origin() const =0;
    virtual const std::array<size_t,3> Region() const =0;
    virtual const char* GetDataType() const =0;
    virtual const bool IsDataType(const char*) const =0;    
    virtual const char* GetObjectType() const =0;
    virtual const bool IsObjectType(const char*) const =0;
};

}

#endif //__cleLightObject_hpp