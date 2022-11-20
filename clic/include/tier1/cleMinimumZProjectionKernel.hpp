

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

inline auto
MinimumZProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MinimumZProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP
