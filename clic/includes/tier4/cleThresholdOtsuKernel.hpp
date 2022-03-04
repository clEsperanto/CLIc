#ifndef __cleThresholdOtsuKernel_hpp
#define __cleThresholdOtsuKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ThresholdOtsuKernel : public Kernel
{
public:
    ThresholdOtsuKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleThresholdOtsuKernel_hpp