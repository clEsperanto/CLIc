

#ifndef CLIC_INCLUDE_TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumYProjectionKernel : public Operation
{
  public:
    explicit MinimumYProjectionKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEMINIMUMYPROJECTIONKERNEL_HPP
