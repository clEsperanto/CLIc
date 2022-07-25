

#ifndef CLIC_INCLUDE_TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumZProjectionKernel : public Operation
{
  public:
    explicit MinimumZProjectionKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEMINIMUMZPROJECTIONKERNEL_HPP
