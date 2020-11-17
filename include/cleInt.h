/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleInt_h
#define __cleInt_h

#include "cleScalar.h"

#include <string>
#include <fstream>

namespace cle
{

class Int : public Scalar
{
private:
    static const ObjectType O = ObjectType::cleInt;
    static const DataType T = DataType::Int;
    int value; 

protected:
    

public:
    Int() = default; 
    Int(int);
    ~Int() = default;

    int& GetData();
    std::string GetDataType() const;
    std::string GetObjectType() const;
    std::string ToString() const;
    bool IsObject(std::string) const;
};

} // namespace cle

#endif // __cleInt_h
