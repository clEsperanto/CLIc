/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __CLE_h
#define __CLE_h



#include "cleGPU.h"
#include "cleObject.h"
#include "image.h"
#include "cleBuffer.h"

#include "utils.h"


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
    void MaximumYProjection(Buffer&, Buffer&);
    void MaximumXProjection(Buffer&, Buffer&);
    void MaximumOfAllPixels(Buffer&, Buffer&);
    void Mean2DSphere(Buffer&, Buffer&, int, int);
    void Absolute(Buffer&, Buffer&);

};


template<class T>
Buffer CLE::Push(Image<T>& image)
{
    cl_mem mem_obj = CreateBuffer(image.GetDataSize(), gpu.GetContextManager().GetContext());
    bool res = WriteBuffer<T>(mem_obj, image.GetData(), image.GetDataSize(), gpu.GetCommandQueueManager().GetCommandQueue());
    return Buffer (mem_obj, image.GetDimensions(), image.GetType());
}

template<class T>
Image<T> CLE::Pull(Buffer& gpu_obj)
{
    unsigned int arrSize = gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    T* output_arr = ReadBuffer<T>(gpu_obj.GetData(), sizeof(T) * arrSize, gpu.GetCommandQueueManager().GetCommandQueue());

    Image<T> image (output_arr, gpu_obj.GetDimensions()[0], gpu_obj.GetDimensions()[1], gpu_obj.GetDimensions()[2], gpu_obj.GetDataType());
    return image;        
}

template<class T>
Buffer CLE::Create(Image<T>& image, std::string type)
{
    cl_mem mem_obj = CreateBuffer(image.GetDataSize(), gpu.GetContextManager().GetContext());
    if (type.empty())
    {
        type = image.GetType();
    }
    return Buffer (mem_obj, image.GetDimensions(), type);
}

template<class T>
Buffer CLE::Create(Buffer& gpu_obj, std::string type)
{
    size_t arrSize = sizeof(T) * gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    cl_mem mem_obj = CreateBuffer(arrSize, gpu.GetContextManager().GetContext());
    if (type.empty())
    {
        type = gpu_obj.GetDataType();
    }
    return Buffer (mem_obj, gpu_obj.GetDimensions(), type);
}

template<class T>
Buffer CLE::Create(unsigned int dimensions[3], std::string type)
{
    size_t arrSize = sizeof(T) * dimensions[0] * dimensions[1] * dimensions[2];
    cl_mem mem_obj = CreateBuffer(arrSize, gpu.GetContextManager().GetContext());
    return Buffer (mem_obj, dimensions, type);
}

} // namespace cle

#endif //__CLE_h