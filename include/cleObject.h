/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleObject_h
#define __cleObject_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


#include "cleLightObject.h"


namespace cle
{

class Object : public LightObject
{
public:
    // enum ObjectType {Buffer, Image2d};
    // enum DataType {Float, Char, UChar, Int, UInt, Short, UShort};

protected:
    // std::string ObjectTypeToString(const ObjectType) const;
    // std::string DataTypeToString(const DataType) const;
    // DataType StringToDataType(const std::string) const;
    

public:
    Object() = default;
    virtual ~Object() = default;

    virtual cl_mem& GetData() = 0;
    virtual unsigned int* GetDimensions() = 0;
    virtual std::string GetDataType() const = 0;
    virtual std::string GetObjectType() const = 0;
    virtual std::string ToString() const = 0;
    virtual bool IsObject(std::string) const = 0;

};

} // namespace cle

#endif // __cleObject_h
