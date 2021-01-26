
#ifndef __cleMaximumYProjectionKernel_h
#define __cleMaximumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumYProjectionKernel : public Kernel
{

public:

    MaximumYProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "maximum_y_projection",
                {"dst_max", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumYProjectionKernel_h
