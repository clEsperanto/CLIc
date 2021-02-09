
#ifndef __cleGreaterOrEqualConstantKernel_h
#define __cleGreaterOrEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class GreaterOrEqualConstantKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    GreaterOrEqualConstantKernel(GPU& gpu) : 
        Kernel( gpu,
                "greater_or_equal_constant",
                {"src1", "scalar", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterOrEqualConstantKernel_h
