#ifndef __TIER1_CLESMALLEROREQUALKERNEL_HPP
#define __TIER1_CLESMALLEROREQUALKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SmallerOrEqualKernel : public Operation
{
public:
  explicit SmallerOrEqualKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLESMALLEROREQUALKERNEL_HPP
