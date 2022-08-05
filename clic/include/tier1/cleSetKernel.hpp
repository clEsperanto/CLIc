
#ifndef __TIER1_CLESETKERNEL_HPP
#define __TIER1_CLESETKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SetKernel : public Operation
{
public:
  explicit SetKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetValue(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLESETKERNEL_HPP
