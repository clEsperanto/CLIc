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

template <class T>
class Scalar : public LightObject
{
private:
    static const ObjectType O = ObjectType::Scalar;
    static const DataType T = DataType::Float;
    T value; 

protected:
    

public:
    Scalar(){};
    
    template<class T>
    Scalar(T v){value = v};
    ~Scalar(){};

    template<class T>
    T& GetData() {return value;}

    std::string GetObjectType() const
    {
        return this->ObjectTypeToString(O);
    };

    std::string ToString() const
    {
        return "";
    };
};

} // namespace cle

#endif // __cleScalar_h
