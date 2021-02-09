
#ifndef __cleSmallerOrEqualConstantKernel_h
#define __cleSmallerOrEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerOrEqualConstantKernel : public Kernel
{

public:
    SmallerOrEqualConstantKernel(GPU& gpu) : 
        Kernel(gpu, 
            "smaller_or_equal_constant",
            {"src1" , "scalar", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetConstant(float);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerOrEqualConstantKernel_h
