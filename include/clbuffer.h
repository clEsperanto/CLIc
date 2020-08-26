#ifndef __clbuffer_h
#define __clbuffer_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <string>
#include <array>

class clBuffer
{

private:
    cl_mem pointer;
    std::array<unsigned int, 3> dimensions = {1, 1, 1};
    std::string type;
    std::string typeId;
    
public:
    clBuffer() = delete;
        
    clBuffer(cl_mem, unsigned int [3], std::string);
    ~clBuffer();

    cl_mem GetPointer();
    std::array<unsigned int, 3> GetDimensions();
    std::string GetType();
    std::string GetTypeId();
    std::string TypeId(std::string);
};

#endif // __clbuffer_h