
#ifndef __TIER1_CLEEQUALCONSTANTKERNEL_HPP
#define __TIER1_CLEEQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class EqualConstantKernel : public Operation
{
public:
  explicit EqualConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLEEQUALCONSTANTKERNEL_HPP
