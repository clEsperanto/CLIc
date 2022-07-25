
#ifndef CLIC_INCLUDE_TIER1_CLEADDIMAGESWEIGHTEDKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEADDIMAGESWEIGHTEDKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AddImagesWeightedKernel : public Operation
{
  public:
    explicit AddImagesWeightedKernel (const ProcessorPointer &device);
    auto SetInput1 (const Image &object) -> void;
    auto SetInput2 (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetFactor1 (const float &value) -> void;
    auto SetFactor2 (const float &value) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEADDIMAGESWEIGHTEDKERNEL_HPP
