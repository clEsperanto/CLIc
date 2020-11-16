/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleBuffer_h
#define __cleBuffer_h

#include "cleObject.h"

#include <array>

namespace cle
{
    
class Buffer : public Object
{

private:
    static const ObjectType O = ObjectType::Buffer;
    DataType T;

    cl_mem pointer = nullptr;
    std::array<unsigned int, 3> dimensions = {0, 0, 0};
    
public:      
    Buffer(){};
    Buffer(cl_mem, unsigned int*, std::string);
    ~Buffer();

    cl_mem& GetData();

    unsigned int* GetDimensions();
    std::string GetObjectType() const;
    std::string GetDataType() const;
    std::string ToString() const;
};



} // namespace cle

#endif // __cleBuffer_h