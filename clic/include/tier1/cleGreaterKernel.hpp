
#ifndef __TIER1_CLEGREATERKERNEL_HPP
#define __TIER1_CLEGREATERKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterKernel : public Operation
{
public:
  explicit GreaterKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEGREATERKERNEL_HPP
