#ifndef __TIER1_CLEMAXIMUMZPROJECTIONKERNEL_HPP
#define __TIER1_CLEMAXIMUMZPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumZProjectionKernel : public Operation
{
public:
  explicit MaximumZProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMAXIMUMZPROJECTIONKERNEL_HPP
