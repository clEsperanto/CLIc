
#ifndef __cleSumXProjectionKernel_hpp
#define __cleSumXProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumXProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_sum_x_projection.h" 
        };

public:
    SumXProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleSumXProjectionKernel_hpp
