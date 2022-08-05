
#ifndef __TIER1_CLEERODESPHEREKERNEL_HPP
#define __TIER1_CLEERODESPHEREKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ErodeSphereKernel : public Operation
{
public:
  ErodeSphereKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEERODESPHEREKERNEL_HPP
