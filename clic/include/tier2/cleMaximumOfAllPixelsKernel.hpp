
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

} // namespace cle

#endif // __TIER2_CLEMAXIMUMOFALLPIXELSKERNEL_HPP
