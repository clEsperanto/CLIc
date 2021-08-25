
#ifndef __cleSumXProjectionKernel_hpp
#define __cleSumXProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumXProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_sum_x_projection.h" 
        ;

public:
    SumXProjectionKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleSumXProjectionKernel_hpp
