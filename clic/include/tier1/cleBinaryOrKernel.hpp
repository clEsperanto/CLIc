
#ifndef __TIER1_CLEBINARYORKERNEL_HPP
#define __TIER1_CLEBINARYORKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryOrKernel : public Operation
{
public:
  explicit BinaryOrKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEBINARYORKERNEL_HPP
