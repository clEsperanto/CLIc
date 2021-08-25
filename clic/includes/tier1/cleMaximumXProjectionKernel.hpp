    
#ifndef __cleMaximumXProjectionKernel_hpp
#define __cleMaximumXProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumXProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_maximum_x_projection.h" 
        ;

public:

    MaximumXProjectionKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumXProjectionKernel_hpp
