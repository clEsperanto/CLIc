

#ifndef __cleMinimumXProjectionKernel_hpp
#define __cleMinimumXProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumXProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_minimum_x_projection.h" 
        ;

public:
    MinimumXProjectionKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumXProjectionKernel_hpp
