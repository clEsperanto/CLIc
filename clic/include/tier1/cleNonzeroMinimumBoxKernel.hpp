

#ifndef __TIER1_CLENONZEROMINIMUMBOXKERNEL_HPP
#define __TIER1_CLENONZEROMINIMUMBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class NonzeroMinimumBoxKernel : public Operation
{
public:
  explicit NonzeroMinimumBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetOutputFlag(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLENONZEROMINIMUMBOXKERNEL_HPP
