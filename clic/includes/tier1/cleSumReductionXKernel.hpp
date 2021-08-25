

#ifndef __cleSumReductionXKernel_hpp
#define __cleSumReductionXKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumReductionXKernel : public Kernel
{
private:

private:
    std::string source = 
        #include "cle_sum_reduction_x.h" 
        ;

public:
    SumReductionXKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetBlocksize(int);
    void Execute();

};

} // namespace cle

#endif // __cleSumReductionXKernel_hpp
