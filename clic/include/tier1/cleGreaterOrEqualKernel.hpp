#ifndef __TIER1_CLEGREATEROREQUALKERNEL_HPP
#define __TIER1_CLEGREATEROREQUALKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterOrEqualKernel : public Operation
{
public:
  explicit GreaterOrEqualKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEGREATEROREQUALKERNEL_HPP
