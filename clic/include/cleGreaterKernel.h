
#ifndef __cleGreaterKernel_h
#define __cleGreaterKernel_h

#include "cleKernel.h"

namespace cle
{
    
class GreaterKernel : public Kernel
{

public:
    GreaterKernel(GPU& gpu) : 
        Kernel( gpu,
                "greater",
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
