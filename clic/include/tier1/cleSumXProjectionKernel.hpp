
#ifndef __TIER1_CLESUMXPROJECTIONKERNEL_HPP
#define __TIER1_CLESUMXPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumXProjectionKernel : public Operation
{
public:
  explicit SumXProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLESUMXPROJECTIONKERNEL_HPP
