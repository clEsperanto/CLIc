
#ifndef CLIC_INCLUDE_TIER1_CLEEQUALCONSTANTKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEEQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class EqualConstantKernel : public Operation
{
  public:
    explicit EqualConstantKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetScalar (const float &value) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEEQUALCONSTANTKERNEL_HPP
