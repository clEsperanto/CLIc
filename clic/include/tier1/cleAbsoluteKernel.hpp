#ifndef __TIER1_CLEABSOLUTEKERNEL_HPP
#define __TIER1_CLEABSOLUTEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AbsoluteKernel : public Operation
{
public:
  explicit AbsoluteKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;

  friend auto
  Absolute_call(AbsoluteKernel * kernel, const Image & src, const Image & dst) -> void;
};

} // namespace cle

#endif // __TIER1_CLEABSOLUTEKERNEL_HPP
