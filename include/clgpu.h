/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __clgpu_h
#define __clgpu_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "clbuffer.h"
#include "image.h"

/** \class clGPU
* \brief Base class for GPU initialisation and data exchange.
*
* clGPU is the GPU initialisation class which manage the gpu paramters
* like device id, context, and commanq_queue. It remains naive for now.
* clGPU is also the class managing the data exchange between the host 
* and the device with the methods push/pull/create.
*/
class clGPU
{

private:
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;

protected:

    void InitialisePlatform();
    void InitialiseDevice();
    void CreateContext();
    void CreateCommandQueue();

public:
    clGPU(){};
    ~clGPU(){};

    void initialisation();

    template<class T>
    clBuffer Push(Image<T>&);
    template<class T>
    Image<T> Pull(clBuffer);
    template<class T>
    clBuffer Create(Image<T>&, std::string = "");
    template<class T>
    clBuffer Create(clBuffer&, std::string = "");
    template<class T>
    clBuffer Create(unsigned int[3], std::string);

    cl_platform_id GetPlateform();
    cl_device_id GetDevice();
    cl_context GetContext();
    cl_command_queue GetCommandQueue();

};

#endif //__clgpu_h