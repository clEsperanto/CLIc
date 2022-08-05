

#ifndef __TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP
#define __TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumZProjectionKernel : public Operation
{
public:
  explicit MinimumZProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP
