
#ifndef __cleMinimumOfAllPixelsKernel_h
#define __cleMinimumOfAllPixelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumOfAllPixelsKernel : public Kernel
{

public:
    MinimumOfAllPixelsKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_of_all_pixels",
                {"dst_min", "src"}
        )
    {}
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumOfAllPixelsKernel_h
