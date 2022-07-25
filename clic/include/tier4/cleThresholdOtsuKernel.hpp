#ifndef CLIC_INCLUDE_TIER4_CLETHRESHOLDOTSUKERNEL_HPP
#define CLIC_INCLUDE_TIER4_CLETHRESHOLDOTSUKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ThresholdOtsuKernel : public Operation
{
  public:
    explicit ThresholdOtsuKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto Execute () -> void override;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER4_CLETHRESHOLDOTSUKERNEL_HPP
