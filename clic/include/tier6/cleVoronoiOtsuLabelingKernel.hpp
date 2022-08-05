#ifndef __TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP
#define __TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class VoronoiOtsuLabelingKernel : public Operation
{
public:
  explicit VoronoiOtsuLabelingKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetSpotSigma(const float & sigma) -> void;
  auto
  SetOutlineSigma(const float & sigma) -> void;
  auto
  Execute() -> void override;

private:
  float spot_sigma_ = 0;
  float output_sigma_ = 0;
};

} // namespace cle

#endif // __TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP
