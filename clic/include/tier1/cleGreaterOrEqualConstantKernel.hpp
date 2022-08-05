
#ifndef __TIER1_CLEGREATEROREQUALCONSTANTKERNEL_HPP
#define __TIER1_CLEGREATEROREQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterOrEqualConstantKernel : public Operation
{
public:
  explicit GreaterOrEqualConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLEGREATEROREQUALCONSTANTKERNEL_HPP
