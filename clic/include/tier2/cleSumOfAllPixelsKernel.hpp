
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

} // namespace cle

#endif // __TIER2_CLESUMOFALLPIXELSKERNEL_HPP
