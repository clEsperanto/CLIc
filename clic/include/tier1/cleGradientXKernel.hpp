#ifndef __TIER1_CLEGRADIENTXKERNEL_HPP
#define __TIER1_CLEGRADIENTXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GradientXKernel : public Operation
{
public:
  explicit GradientXKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;
};

inline auto
GradientXKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  GradientXKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEGradientXKERNEL_HPP
