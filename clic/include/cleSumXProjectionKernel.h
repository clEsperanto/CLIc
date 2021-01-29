
#ifndef __cleSumXProjectionKernel_h
#define __cleSumXProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumXProjectionKernel : public Kernel
{

public:
    SumXProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_x_projection",
                {"dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleSumXProjectionKernel_h
