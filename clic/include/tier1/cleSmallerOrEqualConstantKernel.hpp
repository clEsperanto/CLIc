
#ifndef __TIER1_CLESMALLEROREQUALCONSTANTKERNEL_HPP
#define __TIER1_CLESMALLEROREQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SmallerOrEqualConstantKernel : public Operation
{
public:
  explicit SmallerOrEqualConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetConstant(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLESMALLEROREQUALCONSTANTKERNEL_HPP
