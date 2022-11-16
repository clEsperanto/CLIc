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

inline auto
VoronoiOtsuLabelingKernel_Call(const std::shared_ptr<cle::Processor> & device,
                               const Image &                           src,
                               const Image &                           dst,
                               const float &                           sigma1,
                               const float &                           sigma2) -> void
{
  VoronoiOtsuLabelingKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetSpotSigma(sigma1);
  kernel.SetOutlineSigma(sigma2);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER6_CLEVORONOIOTSULABELINGKERNEL_HPP
