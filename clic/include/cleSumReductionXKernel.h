

#ifndef __cleSumReductionXKernel_h
#define __cleSumReductionXKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumReductionXKernel : public Kernel
{
private:

public:
    SumReductionXKernel(GPU& gpu) : 
        Kernel( gpu,
                "sum_reduction_x",
                {"dst", "src", "blocksize"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetBlocksize(int);
    void Execute();

};

} // namespace cle

#endif // __cleSumReductionXKernel_h
