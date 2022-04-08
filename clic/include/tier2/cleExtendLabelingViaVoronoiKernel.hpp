#ifndef __cleExtendLabelingViaVoronoiKernel_hpp
#define __cleExtendLabelingViaVoronoiKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ExtendLabelingViaVoronoiKernel : public Kernel
{
public:
    ExtendLabelingViaVoronoiKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleExtendLabelingViaVoronoiKernel_hpp