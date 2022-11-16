
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

inline auto
SumXProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  SumXProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESUMXPROJECTIONKERNEL_HPP
