
#ifndef __cleReplaceIntensityKernel_hpp
#define __cleReplaceIntensityKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ReplaceIntensityKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_replace_intensity.h" 
        };

public:
    ReplaceIntensityKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetInValue(float);
    void SetOutValue(float);
    void Execute();
};

} // namespace cle

#endif // __cleReplaceIntensityKernel_hpp
