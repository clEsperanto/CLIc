

#ifndef __cleSumReductionXKernel_hpp
#define __cleSumReductionXKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumReductionXKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_sum_reduction_x.h" 
        };

public:
    SumReductionXKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetBlocksize(int);
    void Execute();
};

} // namespace cle

#endif // __cleSumReductionXKernel_hpp
