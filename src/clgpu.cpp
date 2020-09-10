/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include "clgpu.h"
#include "utils.h"

void clGPU::InitialisePlatform()
    {
        cl_uint ret_num_platforms;
        cl_int clError = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
        if (clError != CL_SUCCESS)  
        {
            std::cerr << "Initialisation error! Could not get platform : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
    }

void clGPU::InitialiseDevice()
{
    cl_uint ret_num_devices;
    cl_int clError = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not get device : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void clGPU::CreateContext()
{
    cl_int clError;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not create context : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void clGPU::CreateCommandQueue()
    {
        cl_int clError;        
#if OCL_MAJOR_VERSION == 2  // 2.0 or higher
        command_queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &clError);
#else 
        command_queue = clCreateCommandQueue(context, device_id, 0, &clError);
#endif
        if (clError != CL_SUCCESS) 
        {
            std::cerr << "Initialisation error! Could not create commande queue : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
    }

void clGPU::initialisation()
    {
        try
        {
            InitialisePlatform();
            InitialiseDevice();
            CreateContext();
            CreateCommandQueue();
        }
        catch(cl_int clError)
        {
            std::cerr << "OCL error caught during initialisation! " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
    }

template<class T>
clBuffer clGPU::Push(Image<T>& image)
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
Image<T> clGPU::Pull(clBuffer gpu_obj)
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
clBuffer clGPU::Create(Image<T>& image, std::string type)
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
clBuffer clGPU::Create(clBuffer& gpu_obj, std::string type)
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
clBuffer clGPU::Create(unsigned int dimensions[3], std::string type)
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

cl_platform_id clGPU::GetPlateform()
{
    return platform_id;
}

cl_device_id clGPU::GetDevice()
{
    return device_id;
}

cl_context clGPU::GetContext()
{
    return context;
}

cl_command_queue clGPU::GetCommandQueue()
{
    return command_queue;
}