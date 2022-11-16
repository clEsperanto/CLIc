

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

inline auto
MinimumXProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MinimumXProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMINIMUMXPROJECTIONKERNEL_HPP
