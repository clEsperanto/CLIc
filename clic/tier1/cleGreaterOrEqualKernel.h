
#ifndef __cleGreaterOrEqualKernel_h
#define __cleGreaterOrEqualKernel_h

#include "cleKernel.h"

namespace cle
{
    
class GreaterOrEqualKernel : public Kernel
{

public:
    GreaterOrEqualKernel(GPU& gpu) : 
        Kernel( gpu,
                "greater_or_equal",
                {"src1", "src2", "dst"}
        )
    {}

    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleGreaterKernel_h
