

#ifndef __cleSmallerOrEqualKernel_h
#define __cleSmallerOrEqualKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerOrEqualKernel : public Kernel
{

public:
    SmallerOrEqualKernel(GPU& gpu) : 
        Kernel( gpu,
                "smaller_or_equal",
                {"src1" , "src2", "dst"}
        )
    {}

    void SetInput1(Buffer&);
    void SetInput2(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerKernel_h
