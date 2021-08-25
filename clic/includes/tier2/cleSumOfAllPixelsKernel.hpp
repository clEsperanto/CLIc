
#ifndef __cleSumOfAllPixelsKernel_hpp
#define __cleSumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SumOfAllPixelsKernel : public Kernel
{

public:
    SumOfAllPixelsKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSumOfAllPixelsKernel_hpp
