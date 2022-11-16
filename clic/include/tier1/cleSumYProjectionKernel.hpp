

#ifndef __TIER1_CLESUMYPROJECTIONKERNEL_HPP
#define __TIER1_CLESUMYPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumYProjectionKernel : public Operation
{
public:
  explicit SumYProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
SumYProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  SumYProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESUMYPROJECTIONKERNEL_HPP
