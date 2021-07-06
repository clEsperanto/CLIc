

#ifndef __cleMinimumKernel_h
#define __cleMinimumKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

    int Radius2KernelSize(float);

public:
    MinimumKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum",
                {"dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

};

} // namespace cle

#endif // __cleMinimumKernel_h
