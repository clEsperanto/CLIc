#ifndef __TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP
#define __TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumYProjectionKernel : public Operation
{
public:
  explicit MaximumYProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP
