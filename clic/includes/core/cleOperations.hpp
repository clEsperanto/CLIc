#ifndef __cleOperations_hpp
#define __cleOperations_hpp

#include "clic.hpp"

#include "cleGPU.hpp"

namespace cle
{
// copy but do not type cast, prefere copy kernel for safety, to be discussed
// template<class T>
// void FillBuffer(cl::Buffer src, T value, size_t size, std::shared_ptr<GPU> gpu)
// {
//     gpu->GetCommandQueueManager().GetCommandQueue().enqueueFillBuffer(
//         src, value, 0, size
//         ); 
// }

template<class T>
void CopyBuffer(cl::Buffer src, cl::Buffer dst, size_t size, std::shared_ptr<GPU> gpu)
{
    try
    {
        gpu->GetCommandQueueManager().GetCommandQueue().enqueueCopyBuffer(
            src, dst, 0, 0, sizeof(T) * size
        );
    }
    catch(cl::Error& e)
    {
        std::cerr << "Operation CopyBuffer : Fail to copy buffer ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

template<class T>
cl::Buffer CreateBuffer(size_t size, std::shared_ptr<GPU> gpu)
{
    return cl::Buffer(
        gpu->GetContextManager().GetContext(), CL_MEM_READ_WRITE, sizeof(T) * size);
}

template<class T>
void WriteBuffer(cl::Buffer obj, T* arr, size_t size, std::shared_ptr<GPU> gpu)
{
    try
    {
        gpu->GetCommandQueueManager().GetCommandQueue().enqueueWriteBuffer(
            obj, CL_TRUE, 0, sizeof(T) * size, arr
        );
    }
    catch(cl::Error& e)
    {
        std::cerr << "Operation WriteBuffer : Fail to write in buffer ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

template<class T>
void ReadBuffer(cl::Buffer obj, T* arr, size_t size, std::shared_ptr<GPU> gpu)
{
    try
    {
        gpu->GetCommandQueueManager().GetCommandQueue().enqueueReadBuffer(
            obj, CL_TRUE, 0, sizeof(T) * size, arr
        );
    }
    catch(cl::Error& e)
    {
        std::cerr << "Operation ReadBuffer : Fail to read buffer ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

template<class T>
cl::Image1D CreateImage1D(int* size, std::shared_ptr<GPU> gpu)
{
    cl::ImageFormat grayscale(CL_INTENSITY, CL_FLOAT);
    return cl::Image1D( gpu->GetContextManager().GetContext(), CL_MEM_READ_WRITE, grayscale, size[0]);
}

template<class T>
cl::Image2D CreateImage2D(int* size, std::shared_ptr<GPU> gpu)
{
    cl::ImageFormat grayscale(CL_INTENSITY, CL_FLOAT);
    return cl::Image2D( gpu->GetContextManager().GetContext(), CL_MEM_READ_WRITE, grayscale, size[0], size[1]);
}

template<class T>
cl::Image3D CreateImage3D(int* size, std::shared_ptr<GPU> gpu)
{
    cl::ImageFormat grayscale(CL_INTENSITY, CL_FLOAT);
    return cl::Image3D( gpu->GetContextManager().GetContext(), CL_MEM_READ_WRITE, grayscale, size[0], size[1], size[2]);
}

template<class T>
void WriteImage(cl::Image* obj, T* arr, std::shared_ptr<GPU> gpu)
{
    cl::size_type row_pitch = obj->getImageInfo<CL_IMAGE_ROW_PITCH>();
    cl::size_type slice_pitch = obj->getImageInfo<CL_IMAGE_SLICE_PITCH>();
    cl::size_type width = obj->getImageInfo<CL_IMAGE_WIDTH>();
    cl::size_type height = obj->getImageInfo<CL_IMAGE_HEIGHT>();
    cl::size_type depth = obj->getImageInfo<CL_IMAGE_DEPTH>(); 

    if(height == 0) height += 1;
    if(depth == 0) depth += 1;
    std::array<cl::size_type,3> origin {0,0,0};
    std::array<cl::size_type,3> region {width, height, depth};

    try
    {
        gpu->GetCommandQueueManager().GetCommandQueue().enqueueWriteImage (
            *obj, CL_TRUE, origin, region, row_pitch, slice_pitch, arr
        );
    }
    catch(cl::Error& e)
    {
        std::cerr << "Operation WriteImage2D : Fail to write image 2d ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

template<class T>
void WriteImage1D(cl::Image1D obj, T* arr, std::shared_ptr<GPU> gpu)
{
    WriteImage<T>(&obj, arr, gpu);
}

template<class T>
void WriteImage2D(cl::Image2D obj, T* arr, std::shared_ptr<GPU> gpu)
{
    WriteImage<T>(&obj, arr, gpu);
}

template<class T>
void WriteImage3D(cl::Image3D obj, T* arr, std::shared_ptr<GPU> gpu)
{
    WriteImage<T>(&obj, arr, gpu);
}  

template<class T>
void ReadImage(cl::Image* obj, T* arr, std::shared_ptr<GPU> gpu)
{
    cl::size_type row_pitch = obj->getImageInfo<CL_IMAGE_ROW_PITCH>();
    cl::size_type slice_pitch = obj->getImageInfo<CL_IMAGE_SLICE_PITCH>(); // should be 0
    cl::size_type width = obj->getImageInfo<CL_IMAGE_WIDTH>();
    cl::size_type height = obj->getImageInfo<CL_IMAGE_HEIGHT>();
    cl::size_type depth = obj->getImageInfo<CL_IMAGE_DEPTH>();  // should be 0

    if(height == 0) height += 1;
    if(depth == 0) depth += 1;
    std::array<cl::size_type,3> origin {0,0,0};
    std::array<cl::size_type,3> region {width, height, depth};

    try
    {
        gpu->GetCommandQueueManager().GetCommandQueue().enqueueReadImage(
            *obj, CL_TRUE, origin, region, row_pitch, slice_pitch, arr
        );
    }
    catch(cl::Error& e)
    {
        std::cerr << "Operation ReadBuffer : Fail to read buffer ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

template<class T>
void ReadImage1D(cl::Image1D obj, T* arr, std::shared_ptr<GPU> gpu)
{
    ReadImage<T>(&obj, arr, gpu);
}

template<class T>
void ReadImage2D(cl::Image2D obj, T* arr, std::shared_ptr<GPU> gpu)
{
    ReadImage<T>(&obj, arr, gpu);
}

template<class T>
void ReadImage3D(cl::Image3D obj, T* arr, std::shared_ptr<GPU> gpu)
{
    ReadImage<T>(&obj, arr, gpu);
}  

} // namespace cle

#endif // __cleOperations_hpp