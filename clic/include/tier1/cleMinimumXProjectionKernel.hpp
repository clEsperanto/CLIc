

#ifndef __TIER1_CLEMINIMUMXPROJECTIONKERNEL_HPP
#define __TIER1_CLEMINIMUMXPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumXProjectionKernel : public Operation
{
public:
  explicit MinimumXProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMINIMUMXPROJECTIONKERNEL_HPP
