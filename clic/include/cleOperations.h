#ifndef __cleOperations_h
#define __cleOperations_h

#ifndef CL_HPP_ENABLE_EXCEPTIONS
#   define CL_HPP_ENABLE_EXCEPTIONS
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#   define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#   define CL_HPP_MINIMUM_OPENCL_VERSION 120
#endif

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#ifdef __APPLE__
#   include <OpenCL/opencl.hpp>
#else
#   include <CL/cl2.hpp>
#endif

#include "cleGPU.h"

namespace cle
{

    template<class T>
    void FillBuffer(cl::Buffer src, T value, size_t size, GPU gpu)
    {
        gpu.GetCommandQueueManager().GetCommandQueue().enqueueFillBuffer(
            src, value, 0, size
            ); 
    }

    template<class T>
    void CopyBuffer(cl::Buffer src, cl::Buffer dst, size_t size, GPU gpu)
    {
        gpu.GetCommandQueueManager().GetCommandQueue().enqueueCopyBuffer(
            src, dst, 0, 0, sizeof(T) * size
        );
    }

    template<class T>
    cl::Buffer CreateBuffer(size_t size, GPU gpu)
    {
        return cl::Buffer(
            gpu.GetContextManager().GetContext(), CL_MEM_READ_WRITE, sizeof(T) * size);
    }

    template<class T>
    void WriteBuffer(cl::Buffer obj, T* arr, size_t size, GPU gpu)
    {
        gpu.GetCommandQueueManager().GetCommandQueue().enqueueWriteBuffer(
            obj, CL_TRUE, 0, sizeof(T) * size, arr
        );
    }

    template<class T>
    void WriteBuffer(cl::Buffer obj, T val, GPU gpu)
    {
        gpu.GetCommandQueueManager().GetCommandQueue().enqueueWriteBuffer(
            obj, CL_TRUE, 0, sizeof(T) * 1, val
        );
    }

    template<class T>
    void ReadBuffer(cl::Buffer obj, T* arr, size_t size, GPU gpu)
    {
        gpu.GetCommandQueueManager().GetCommandQueue().enqueueReadBuffer(
            obj, CL_TRUE, 0, sizeof(T) * size, arr
        );
    }

} // namespace cle

#endif // __cleOperations_h