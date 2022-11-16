#ifndef __TIER5_CLEMASKEDVORONOILABELINGKERNEL_HPP
#define __TIER5_CLEMASKEDVORONOILABELINGKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaskedVoronoiLabelingKernel : public Operation
{
public:
  explicit MaskedVoronoiLabelingKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetMask(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
MaskedVoronoiLabelingKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                 const Image &                           src,
                                 const Image &                           dst,
                                 const Image &                           mask) -> void
{
  MaskedVoronoiLabelingKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetMask(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER5_CLEMASKEDVORONOILABELINGKERNEL_HPP
