
#ifndef __cleEqualConstantKernel_h
#define __cleEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class EqualConstantKernel : public Kernel
{

public:
    EqualConstantKernel(GPU& gpu) : 
        Kernel( gpu, 
                "equal_constant",
                {"src1", "scalar", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleEqualConstantKernel_h
