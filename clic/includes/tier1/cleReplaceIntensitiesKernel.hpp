

#ifndef __cleReplaceIntensitiesKernel_hpp
#define __cleReplaceIntensitiesKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ReplaceIntensitiesKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_replace_intensities.h" 
        ;

public:
    ReplaceIntensitiesKernel (std::shared_ptr<GPU>);  
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetMap(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleReplaceIntensitiesKernel_hpp
