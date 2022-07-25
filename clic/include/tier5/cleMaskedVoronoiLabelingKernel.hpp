#ifndef CLIC_INCLUDE_TIER5_CLEMASKEDVORONOILABELINGKERNEL_HPP
#define CLIC_INCLUDE_TIER5_CLEMASKEDVORONOILABELINGKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaskedVoronoiLabelingKernel : public Operation
{
  public:
    explicit MaskedVoronoiLabelingKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetMask (const Image &object) -> void;
    auto Execute () -> void override;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER5_CLEMASKEDVORONOILABELINGKERNEL_HPP
