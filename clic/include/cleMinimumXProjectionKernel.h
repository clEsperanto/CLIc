

#ifndef __cleMinimumXProjectionKernel_h
#define __cleMinimumXProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumXProjectionKernel : public Kernel
{

public:
    MinimumXProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_x_projection",
                {"dst_min", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumXProjectionKernel_h
