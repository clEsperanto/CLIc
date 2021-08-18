#ifndef __cleOperations_hpp
#define __cleOperations_hpp

#include "clic.hpp"

#include "cleGPU.hpp"

namespace cle
{
    // Removed for now, prefere to use kernel operation Set()
    // template<class T>
    // void FillBuffer(cl::Buffer src, T value, size_t size, GPU gpu)
    // {
    //     gpu.GetCommandQueueManager().GetCommandQueue().enqueueFillBuffer(
    //         src, value, 0, size
    //         ); 
    // }

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

#endif // __cleOperations_hpp