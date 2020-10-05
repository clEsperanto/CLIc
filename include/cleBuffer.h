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

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <string>
#include <array>

namespace cle
{
    
class Buffer
{

private:
    cl_mem pointer = nullptr;
    std::array<unsigned int, 3> dimensions = {0, 0, 0};
    std::string type = "";
    std::string typeId = "";

    friend std::ostream & operator<<(std::ostream &, const Buffer&);
    
public:
    Buffer(){};
        
    Buffer(cl_mem, unsigned int [3], std::string);
    ~Buffer();

    cl_mem GetPointer();
    std::array<unsigned int, 3> GetDimensions();
    std::string GetType();
    std::string GetTypeId();
    std::string TypeId(std::string);

    std::string to_str() const;
};



} // namespace cle

#endif // __cleBuffer_h