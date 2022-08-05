
#ifndef __TIER1_CLEBINARYXORKERNEL_HPP
#define __TIER1_CLEBINARYXORKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryXorKernel : public Operation
{
public:
  explicit BinaryXorKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEBINARYXORKERNEL_HPP
