

#ifndef __cleSumYProjectionKernel_hpp
#define __cleSumYProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumYProjectionKernel : public Kernel
{

private:
    std::string source = 
        #include "cle_sum_y_projection.h" 
        ;

public:
    SumYProjectionKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSumYProjectionKernel_hpp
