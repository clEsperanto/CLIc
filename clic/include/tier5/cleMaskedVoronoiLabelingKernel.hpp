#ifndef __cleMaskedVoronoiLabelingKernel_hpp
#define __cleMaskedVoronoiLabelingKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaskedVoronoiLabelingKernel : public Kernel
{
public:
    MaskedVoronoiLabelingKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetMask(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleMaskedVoronoiLabelingKernel_hpp