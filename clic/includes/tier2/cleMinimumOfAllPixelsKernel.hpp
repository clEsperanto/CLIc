
#ifndef __cleMinimumOfAllPixelsKernel_hpp
#define __cleMinimumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumOfAllPixelsKernel : public Kernel
{

public:
    MinimumOfAllPixelsKernel (std::shared_ptr<GPU> gpu);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumOfAllPixelsKernel_hpp
