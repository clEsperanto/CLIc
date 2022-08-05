#ifndef __TIER1_CLEMASKKERNEL_HPP
#define __TIER1_CLEMASKKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaskKernel : public Operation
{
public:
  explicit MaskKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetMask(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMASKKERNEL_HPP
