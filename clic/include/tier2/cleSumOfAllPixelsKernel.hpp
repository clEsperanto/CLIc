
#ifndef __TIER2_CLESUMOFALLPIXELSKERNEL_HPP
#define __TIER2_CLESUMOFALLPIXELSKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumOfAllPixelsKernel : public Operation
{
public:
  explicit SumOfAllPixelsKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
SumOfAllPixelsKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  SumOfAllPixelsKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER2_CLESUMOFALLPIXELSKERNEL_HPP
