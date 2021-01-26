

#ifndef __cleNotEqualConstantKernel_h
#define __cleNotEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class NotEqualConstantKernel : public Kernel
{

public:
    NotEqualConstantKernel(GPU& gpu) : 
        Kernel( gpu,
                "not_equal_constant",
                {"src1", "scalar", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();

};

} // namespace cle

#endif // __cleNotEqualConstantKernel_h
