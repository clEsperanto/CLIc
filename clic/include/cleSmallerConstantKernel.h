
#ifndef __cleSmallerConstantKernel_h
#define __cleSmallerConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerConstantKernel : public Kernel
{

public:
    SmallerConstantKernel(GPU& gpu) : 
        Kernel( gpu,
                "smaller_constant",
                {"src1" , "scalar", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetConstant(float);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerConstantKernel_h
