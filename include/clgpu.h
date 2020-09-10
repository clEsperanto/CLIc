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
#include "utils.h"

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

    void Initialisation();

    cl_platform_id GetPlateform();
    cl_device_id GetDevice();
    cl_context GetContext();
    cl_command_queue GetCommandQueue();

    template<class T>
    clBuffer Push(Image<T>& image)
    {
        cl_int clError;
        cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, image.GetDataSize(), image.GetData(), &clError);
        if (clError != CL_SUCCESS)
        {
            std::cerr << "Push error! fail to create buffer : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
        clError = clEnqueueWriteBuffer(command_queue, mem_obj, CL_TRUE, 0, image.GetDataSize(), image.GetData(), 0, nullptr, nullptr);
        if (clError != CL_SUCCESS)
        {
            std::cerr << "Push error! fail to write buffer in push() " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
        return clBuffer (mem_obj, image.GetDimensions().data(), image.GetType());
    }

    template<class T>
    Image<T> Pull(clBuffer gpu_obj)
    {
        unsigned int arrSize = gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
        size_t bitSize = sizeof(T) * arrSize;
        T* output_arr = new T[arrSize];
        cl_int clError = clEnqueueReadBuffer(command_queue, gpu_obj.GetPointer(), CL_TRUE, 0, bitSize, output_arr, 0, NULL, NULL);
        if (clError != CL_SUCCESS)
        {
            std::cerr << "Pull error! fail to read buffer : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
        Image<T> image (output_arr, gpu_obj.GetDimensions()[0], gpu_obj.GetDimensions()[1], gpu_obj.GetDimensions()[2], gpu_obj.GetType());
        return image;        
    }

    template<class T>
    clBuffer Create(Image<T>& image, std::string type = "")
    {
        cl_int clError;
        cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, image.GetDataSize(), image.GetData(), &clError);
        if (clError != CL_SUCCESS)
        {
            std::cerr << "Create error! fail to create buffer : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
        if (type.empty())
        {
            type = image.GetType();
        }
        return clBuffer (mem_obj, image.GetDimensions().data(), type);
    }

    template<class T>
    clBuffer Create(clBuffer& gpu_obj, std::string type = "")
    {
        size_t arrSize = sizeof(T) * gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
        cl_int clError;
        cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, arrSize, nullptr, &clError);
        if (clError != CL_SUCCESS)
        {
            std::cerr << "Create error! fail to create buffer : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
        if (type.empty())
        {
            type = gpu_obj.GetType();
        }
        return clBuffer (mem_obj, gpu_obj.GetDimensions().data(), type);
    }

    template<class T>
    clBuffer Create(unsigned int dimensions[3], std::string type)
    {
        size_t arrSize = sizeof(T) * dimensions[0] * dimensions[1] * dimensions[2];
        cl_int clError;
        cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, arrSize, nullptr, &clError);
        if (clError != CL_SUCCESS)
        {
            std::cerr << "OCL Error! fail to create buffer in create() : " << getOpenCLErrorString(clError) << std::endl;
        }
        return clBuffer (mem_obj, dimensions, type);
    }



};

#endif //__clgpu_h