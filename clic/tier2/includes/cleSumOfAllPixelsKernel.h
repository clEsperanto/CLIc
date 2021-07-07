
#ifndef __cleSumOfAllPixelsKernel_h
#define __cleSumOfAllPixelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumOfAllPixelsKernel : public Kernel
{

public:
    SumOfAllPixelsKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_of_all_pixels",
                {"dst_sum", "src"}
        )
    {}
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSumOfAllPixelsKernel_h
