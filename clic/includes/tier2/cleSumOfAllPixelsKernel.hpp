
#ifndef __cleSumOfAllPixelsKernel_hpp
#define __cleSumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

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

#endif // __cleSumOfAllPixelsKernel_hpp
