
#ifndef __cleMaximumOfAllPixelsKernel_hpp
#define __cleMaximumOfAllPixelsKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumOfAllPixelsKernel : public Kernel
{

public:

    MaximumOfAllPixelsKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleMaximumOfAllPixelsKernel_hpp
