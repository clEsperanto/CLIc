#ifndef __cleLightObject_h
#define __cleLightObject_h

#ifndef CL_HPP_ENABLE_EXCEPTIONS
#   define CL_HPP_ENABLE_EXCEPTIONS
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#   define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#   define CL_HPP_MINIMUM_OPENCL_VERSION 120
#endif

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#ifdef __APPLE__
#   include <OpenCL/cl.h>
#   include <OpenCL/cl2.hpp>
#else
#   include <CL/cl.h>
#   include <CL/cl2.hpp>
#endif

#include <string>
#include <fstream>

namespace cle
{
    
class LightObject
{   

public:
    enum ObjectType {cleBuffer, cleFloat, cleInt}; 
    enum DataType {Float, Char, UChar, Int, UInt, Short, UShort};

    LightObject() = default;
    ~LightObject() = default;

    virtual std::string GetObjectType() const = 0;
    virtual bool IsObject(ObjectType) const = 0;
    virtual size_t GetBitSize() const = 0;

protected:    
    std::string ObjectTypeToString(const ObjectType) const;
    std::string DataTypeToString(const DataType) const;
    DataType StringToDataType(const std::string) const;

};

} // namespace cle

#endif // __cleLightObject_h