

#ifndef __cleSumYProjectionKernel_h
#define __cleSumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumYProjectionKernel : public Kernel
{

public:
    SumYProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_y_projection",
                {"dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSumYProjectionKernel_h
