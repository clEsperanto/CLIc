    
#ifndef __cleMaximumXProjectionKernel_hpp
#define __cleMaximumXProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumXProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_maximum_x_projection.h" 
        };

public:
    MaximumXProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumXProjectionKernel_hpp
