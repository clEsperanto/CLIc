
#ifndef __cleEqualKernel_h
#define __cleEqualKernel_h

#include "cleKernel.h"

namespace cle
{
    
class EqualKernel : public Kernel
{

public:

    EqualKernel(GPU& gpu) : 
        Kernel( gpu,
                "equal",
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
