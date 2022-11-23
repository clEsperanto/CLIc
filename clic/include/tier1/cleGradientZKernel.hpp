#ifndef __TIER1_CLEGRADIENTZKERNEL_HPP
#define __TIER1_CLEGRADIENTZKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GradientZKernel : public Operation
{
public:
  explicit GradientZKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;
};

inline auto
GradientZKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  GradientZKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEGradientZKERNEL_HPP
