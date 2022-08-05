

#ifndef __TIER1_CLESUMZPROJECTIONKERNEL_HPP
#define __TIER1_CLESUMZPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumZProjectionKernel : public Operation
{
public:
  explicit SumZProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLESUMZPROJECTIONKERNEL_HPP
