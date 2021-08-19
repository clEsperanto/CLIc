
#ifndef __cleMaximumOfAllPixelsKernel_hpp
#define __cleMaximumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumOfAllPixelsKernel : public Kernel
{

public:

    MaximumOfAllPixelsKernel (std::shared_ptr<GPU> gpu) : 
        Kernel( gpu, 
                "maximum_of_all_pixels",
                {"dst_max", "src"}
        )
    {}
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumOfAllPixelsKernel_hpp
