/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleFloat_h
#define __cleFloat_h

#include "cleScalar.h"

#include <string>
#include <fstream>

namespace cle
{

class Float : public Scalar
{
private:
    static const ObjectType O = ObjectType::cleFloat;
    static const DataType T = DataType::Float;
    float value; 

protected:
    

public:
    Float() = default;
    Float(float);
    ~Float() = default;

    float& GetData();
    std::string GetObjectType() const;
    std::string ToString() const;
    bool IsObject(std::string) const;

};

} // namespace cle

#endif // __cleScalar_h
