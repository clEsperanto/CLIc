
#ifndef __TIER2_CLEMINIMUMOFALLPIXELSKERNEL_HPP
#define __TIER2_CLEMINIMUMOFALLPIXELSKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumOfAllPixelsKernel : public Operation
{
public:
  explicit MinimumOfAllPixelsKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
MinimumOfAllPixelsKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MinimumOfAllPixelsKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER2_CLEMINIMUMOFALLPIXELSKERNEL_HPP
