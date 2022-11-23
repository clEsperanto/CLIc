#ifndef __TIER1_CLEGRADIENTYKERNEL_HPP
#define __TIER1_CLEGRADIENTYKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GradientYKernel : public Operation
{
public:
  explicit GradientYKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;
};

inline auto
GradientYKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  GradientYKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEGRADIENTYKERNEL_HPP
