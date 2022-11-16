

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

inline auto
MinimumYProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MinimumYProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP
