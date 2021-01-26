
#ifndef __cleMaximumXProjectionKernel_h
#define __cleMaximumXProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumXProjectionKernel : public Kernel
{

public:

    MaximumXProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "maximum_x_projection",
                {"dst_max", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumXProjectionKernel_h
