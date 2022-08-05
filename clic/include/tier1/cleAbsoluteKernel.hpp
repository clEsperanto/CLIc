#ifndef __TIER1_CLEABSOLUTEKERNEL_HPP
#define __TIER1_CLEABSOLUTEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AbsoluteKernel : public Operation
{
public:
  explicit AbsoluteKernel(const ProcessorPointer & device);
  ~AbsoluteKernel() = default;
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEABSOLUTEKERNEL_HPP
