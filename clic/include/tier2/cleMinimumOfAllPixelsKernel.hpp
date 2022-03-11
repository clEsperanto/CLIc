
#ifndef __cleMinimumOfAllPixelsKernel_hpp
#define __cleMinimumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumOfAllPixelsKernel : public Kernel
{
public:
    MinimumOfAllPixelsKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMinimumOfAllPixelsKernel_hpp
