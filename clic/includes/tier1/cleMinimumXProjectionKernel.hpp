

#ifndef __cleMinimumXProjectionKernel_hpp
#define __cleMinimumXProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumXProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_minimum_x_projection.h" 
        };

public:
    MinimumXProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumXProjectionKernel_hpp
