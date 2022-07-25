#ifndef CLIC_INCLUDE_TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP
#define CLIC_INCLUDE_TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class VoronoiOtsuLabelingKernel : public Operation
{
  public:
    explicit VoronoiOtsuLabelingKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetSpotSigma (const float &sigma) -> void;
    auto SetOutlineSigma (const float &sigma) -> void;
    auto Execute () -> void override;

  private:
    float spot_sigma_;
    float output_sigma_;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP
