/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleScalar_h
#define __cleScalar_h

#include "cleLightObject.h"

#include <string>
#include <fstream>

namespace cle
{

class Scalar : public LightObject
{
private:
    // static const ObjectType O = ObjectType::Scalar;
    // static const DataType T = DataType::Float;
    // T value; 

protected:
    

public:
    Scalar() = default;
    virtual ~Scalar() = default;

    virtual std::string GetObjectType() const = 0;
    virtual std::string ToString() const = 0;
    virtual bool IsObject(std::string) const = 0;
    virtual size_t GetBitSize() const = 0;

};

} // namespace cle

#endif // __cleScalar_h
