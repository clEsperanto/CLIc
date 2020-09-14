/**
 * Author: Stephane Rigaud - @strigaud 
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
    cl_mem pointer;
    std::array<unsigned int, 3> dimensions = {1, 1, 1};
    std::string type;
    std::string typeId;
    
public:
    Buffer() = delete;
        
    Buffer(cl_mem, unsigned int [3], std::string);
    ~Buffer();

    cl_mem GetPointer();
    std::array<unsigned int, 3> GetDimensions();
    std::string GetType();
    std::string GetTypeId();
    std::string TypeId(std::string);
};

} // namespace cle

#endif // __cleBuffer_h