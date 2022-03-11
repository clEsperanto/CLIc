

#ifndef __cleReplaceIntensitiesKernel_hpp
#define __cleReplaceIntensitiesKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ReplaceIntensitiesKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_replace_intensities.h" 
        };

public:
    ReplaceIntensitiesKernel(std::shared_ptr<GPU>);  
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetMap(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleReplaceIntensitiesKernel_hpp
