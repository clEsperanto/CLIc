
#ifndef __cleSetKernel_h
#define __cleSetKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SetKernel : public Kernel
{

public:
    SetKernel(GPU& gpu) : 
        Kernel( gpu,
                "set",
                {"dst", "value"}
        )
    {}

    void SetInput(Buffer&);
    void SetValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleSetKernel_h
