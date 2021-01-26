
#ifndef __cleMaximumKernel_h
#define __cleMaximumKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

    int Radius2KernelSize(float);

public:
    MaximumKernel(GPU& gpu) : 
        Kernel( gpu,
                "maximum",
                {"dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumKernel_h
