

#ifndef __cleSumZProjectionKernel_h
#define __cleSumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumZProjectionKernel : public Kernel
{

public:
    SumZProjectionKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_z_projection",
                {"dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleSumZProjectionKernel_h
