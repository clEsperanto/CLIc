
#ifndef __cleMaximumZProjectionKernel_hpp
#define __cleMaximumZProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumZProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_maximum_z_projection.h" 
        };

public:
    MaximumZProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumZProjectionKernel_hpp
