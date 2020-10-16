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

#include "cleObject.h"

#include <array>

namespace cle
{
    
class Scalar : public Object
{

private:
    static const ObjectType O = ObjectType::Scalar;
    static const DataType T = DataType::Float;
    float value;
    
public:        
    Scalar(float);
    ~Scalar();
    float GetValue();

    std::string GetObjectType() const;
    std::string GetDataType() const;
    std::string ToString() const;
};



} // namespace cle

#endif // __cleScalar_h