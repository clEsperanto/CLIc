
#ifndef __TIER1_CLEBINARYANDKERNEL_HPP
#define __TIER1_CLEBINARYANDKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryAndKernel : public Operation
{
public:
  explicit BinaryAndKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEBINARYANDKERNEL_HPP
