
#ifndef __TIER2_CLEMAXIMUMOFALLPIXELSKERNEL_HPP
#define __TIER2_CLEMAXIMUMOFALLPIXELSKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumOfAllPixelsKernel : public Operation
{
public:
  explicit MaximumOfAllPixelsKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
MaximumOfAllPixelsKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MaximumOfAllPixelsKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER2_CLEMAXIMUMOFALLPIXELSKERNEL_HPP
