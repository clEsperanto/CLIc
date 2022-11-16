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

inline auto
MaximumYProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MaximumYProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP
