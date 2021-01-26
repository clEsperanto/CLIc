
#ifndef __cleGreaterConstantKernel_h
#define __cleGreaterConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class GreaterConstantKernel : public Kernel
{

public:
    GreaterConstantKernel(GPU& gpu) : 
        Kernel( gpu,
                "greater_constant",
                {"src1", "scalar", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();

};

} // namespace cle

#endif // __cleGreaterConstantKernel_h
