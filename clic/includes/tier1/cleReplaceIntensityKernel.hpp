
#ifndef __cleReplaceIntensityKernel_hpp
#define __cleReplaceIntensityKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ReplaceIntensityKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_replace_intensity.h" 
        ;

public:
    ReplaceIntensityKernel (GPU* gpu) : 
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

#endif // __cleReplaceIntensityKernel_hpp
