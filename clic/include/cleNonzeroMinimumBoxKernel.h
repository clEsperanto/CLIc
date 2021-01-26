

#ifndef __cleNonzeroMinimumBoxKernel_h
#define __cleNonzeroMinimumBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class NonzeroMinimumBoxKernel : public Kernel
{

public:
    NonzeroMinimumBoxKernel(GPU& gpu) : 
        Kernel(gpu,
            "nonzero_minimum_box",
            {"dst", "flag_dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetOutputFlag(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleNonzeroMinimumBoxKernel_h
