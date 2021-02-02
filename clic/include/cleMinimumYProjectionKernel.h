

#ifndef __cleMinimumYProjectionKernel_h
#define __cleMinimumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumYProjectionKernel : public Kernel
{

public:
    MinimumYProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_y_projection",
                {"dst_min", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleMinimumYProjectionKernel_h
