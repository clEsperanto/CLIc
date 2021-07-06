
#ifndef __cleSmallerKernel_h
#define __cleSmallerKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerKernel : public Kernel
{

public:
    SmallerKernel(GPU& gpu) : 
        Kernel( gpu, 
                "smaller",
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
