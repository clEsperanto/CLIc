
#ifndef __TIER1_CLESUBTRACTIMAGEFROMSCALARKERNEL_HPP
#define __TIER1_CLESUBTRACTIMAGEFROMSCALARKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SubtractImageFromScalarKernel : public Operation
{
public:
  explicit SubtractImageFromScalarKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

inline auto
SubtractImageFromScalarKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                   const Image &                           src,
                                   const Image &                           dst,
                                   const float &                           value) -> void
{
  SubtractImageFromScalarKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetScalar(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESUBTRACTIMAGEFROMSCALARKERNEL_HPP
