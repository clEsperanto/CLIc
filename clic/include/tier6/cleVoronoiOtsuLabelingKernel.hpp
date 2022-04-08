#ifndef __cleVoronoiOtsuLabelingKernel_hpp
#define __cleVoronoiOtsuLabelingKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class VoronoiOtsuLabelingKernel : public Kernel
{
public:
    VoronoiOtsuLabelingKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSpotSigma(float=2);
    void SetOutlineSigma(float=2);
    void Execute();

private:
    float m_SpotSigma;
    float m_OutlineSigma;
};

} // namespace cle

#endif // __cleVoronoiOtsuLabelingKernel_hpp