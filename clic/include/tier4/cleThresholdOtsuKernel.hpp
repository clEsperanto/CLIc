#ifndef __TIER4_CLETHRESHOLDOTSUKERNEL_HPP
#define __TIER4_CLETHRESHOLDOTSUKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ThresholdOtsuKernel : public Operation
{
public:
  explicit ThresholdOtsuKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
ThresholdOtsuKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  ThresholdOtsuKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER4_CLETHRESHOLDOTSUKERNEL_HPP
