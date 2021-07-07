
#ifndef __cleReplaceIntensityKernel_h
#define __cleReplaceIntensityKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ReplaceIntensityKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_replace_intensity.h" 
        ;

public:
    ReplaceIntensityKernel(GPU& gpu) : 
        Kernel(gpu,
            "replace_intensity",
            {"dst", "src", "in", "out"}
        )
    {
        m_Sources.insert({this->m_KernelName + "", source});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetInValue(float);
    void SetOutValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleReplaceIntensityKernel_h
