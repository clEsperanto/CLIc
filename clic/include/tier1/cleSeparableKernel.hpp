#ifndef CLIC_INCLUDE_TIER1_CLESEPARABLEKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLESEPARABLEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SeparableKernel : public Operation
{
  public:
    explicit SeparableKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetSigma (const float &sigma) -> void;
    auto SetSize (const int &radius) -> void;
    auto SetDimension (const int &dimension) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLESEPARABLEKERNEL_HPP
