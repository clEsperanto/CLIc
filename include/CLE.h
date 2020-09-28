
#ifndef __CLE_h
#define __CLE_h


#include "cleGPU.h"
#include "cleBuffer.h"
#include "image.h"


namespace cle
{

class CLE
{
private:
    GPU gpu;

public:
    CLE(GPU&);
    ~CLE(){};

    template<class T>
    Buffer Push(Image<T>& image);
    template<class T>
    Image<T> Pull(Buffer& gpu_obj);
    template<class T>
    Buffer Create(Image<T>& image, std::string type = "");
    template<class T>
    Buffer Create(Buffer& gpu_obj, std::string type = "");
    template<class T>
    Buffer Create(unsigned int dimensions[3], std::string type);

    GPU GetGPU();

    void AddImageAndScalar(Buffer&, Buffer&, float);
    void SmallerOrEqualConstant(Buffer&, Buffer&, float);
    void MaximumZProjection(Buffer&, Buffer&);
    void Absolute(Buffer&, Buffer&);

};


template<class T>
Buffer CLE::Push(Image<T>& image)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(gpu.GetContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, image.GetDataSize(), image.GetData(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Push error! fail to create buffer : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    clError = clEnqueueWriteBuffer(gpu.GetCommandQueue(), mem_obj, CL_TRUE, 0, image.GetDataSize(), image.GetData(), 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Push error! fail to write buffer in push() " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    return Buffer (mem_obj, image.GetDimensions().data(), image.GetType());
}

template<class T>
Image<T> CLE::Pull(Buffer& gpu_obj)
{
    unsigned int arrSize = gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    size_t bitSize = sizeof(T) * arrSize;
    T* output_arr = new T[arrSize];
    cl_int clError = clEnqueueReadBuffer(gpu.GetCommandQueue(), gpu_obj.GetPointer(), CL_TRUE, 0, bitSize, output_arr, 0, NULL, NULL);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Pull error! fail to read buffer : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    Image<T> image (output_arr, gpu_obj.GetDimensions()[0], gpu_obj.GetDimensions()[1], gpu_obj.GetDimensions()[2], gpu_obj.GetType());
    return image;        
}

template<class T>
Buffer CLE::Create(Image<T>& image, std::string type)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(gpu.GetContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, image.GetDataSize(), image.GetData(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Create error! fail to create buffer : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    if (type.empty())
    {
        type = image.GetType();
    }
    return Buffer (mem_obj, image.GetDimensions().data(), type);
}

template<class T>
Buffer CLE::Create(Buffer& gpu_obj, std::string type)
{
    size_t arrSize = sizeof(T) * gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(gpu.GetContext(), CL_MEM_READ_ONLY, arrSize, nullptr, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Create error! fail to create buffer : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    if (type.empty())
    {
        type = gpu_obj.GetType();
    }
    return Buffer (mem_obj, gpu_obj.GetDimensions().data(), type);
}

template<class T>
Buffer CLE::Create(unsigned int dimensions[3], std::string type)
{
    size_t arrSize = sizeof(T) * dimensions[0] * dimensions[1] * dimensions[2];
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(gpu.GetContext(), CL_MEM_READ_ONLY, arrSize, nullptr, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "OCL Error! fail to create buffer in create() : " << getOpenCLErrorString(clError) << std::endl;
    }
    return Buffer (mem_obj, dimensions, type);
}




} // namespace cle

#endif //__CLE_h