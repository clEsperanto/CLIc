
#ifndef __cleMinimumOfAllPixelsKernel_hpp
#define __cleMinimumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumOfAllPixelsKernel : public Kernel
{

public:
    MinimumOfAllPixelsKernel (GPU* gpu) : 
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

#endif // __cleMinimumOfAllPixelsKernel_hpp
