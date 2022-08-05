

#ifndef __TIER1_CLENOTEQUALCONSTANTKERNEL_HPP
#define __TIER1_CLENOTEQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class NotEqualConstantKernel : public Operation
{
public:
  explicit NotEqualConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLENOTEQUALCONSTANTKERNEL_HPP
