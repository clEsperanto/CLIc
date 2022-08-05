
#ifndef __TIER1_CLEGREATERCONSTANTKERNEL_HPP
#define __TIER1_CLEGREATERCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterConstantKernel : public Operation
{
public:
  explicit GreaterConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLEGREATERCONSTANTKERNEL_HPP
