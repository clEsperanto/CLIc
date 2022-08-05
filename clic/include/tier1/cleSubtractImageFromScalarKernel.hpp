
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

} // namespace cle

#endif // __TIER1_CLESUBTRACTIMAGEFROMSCALARKERNEL_HPP
