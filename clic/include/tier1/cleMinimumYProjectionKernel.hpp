

#ifndef __TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP
#define __TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumYProjectionKernel : public Operation
{
public:
  explicit MinimumYProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP
