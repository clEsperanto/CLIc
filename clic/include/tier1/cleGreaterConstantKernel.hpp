
#ifndef CLIC_INCLUDE_TIER1_CLEGREATERCONSTANTKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEGREATERCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterConstantKernel : public Operation
{
  public:
    explicit GreaterConstantKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetScalar (const float &value) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEGREATERCONSTANTKERNEL_HPP
