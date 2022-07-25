
#ifndef CLIC_INCLUDE_TIER2_CLEMINIMUMOFALLPIXELSKERNEL_HPP
#define CLIC_INCLUDE_TIER2_CLEMINIMUMOFALLPIXELSKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumOfAllPixelsKernel : public Operation
{
  public:
    explicit MinimumOfAllPixelsKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto Execute () -> void override;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER2_CLEMINIMUMOFALLPIXELSKERNEL_HPP
