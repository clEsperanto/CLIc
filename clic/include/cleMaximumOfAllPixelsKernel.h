
#ifndef __cleMaximumOfAllPixelsKernel_h
#define __cleMaximumOfAllPixelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumOfAllPixelsKernel : public Kernel
{

public:

    MaximumOfAllPixelsKernel(GPU& gpu) : 
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

#endif // __cleMaximumOfAllPixelsKernel_h
