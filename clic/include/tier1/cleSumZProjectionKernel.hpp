

#ifndef __cleSumZProjectionKernel_hpp
#define __cleSumZProjectionKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumZProjectionKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_sum_z_projection.h" 
        };

public:
    SumZProjectionKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleSumZProjectionKernel_hpp
