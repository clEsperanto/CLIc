
#ifndef __cleReplaceIntensityKernel_h
#define __cleReplaceIntensityKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ReplaceIntensityKernel : public Kernel
{

public:
    ReplaceIntensityKernel(GPU& gpu) : 
        Kernel(gpu,
            "replace_intensity",
            {"dst", "src", "in", "out"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetInValue(float);
    void SetOutValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleReplaceIntensityKernel_h
