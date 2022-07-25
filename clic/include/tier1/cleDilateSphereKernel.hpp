
#ifndef CLIC_INCLUDE_TIER1_CLEDILATESPHEREKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEDILATESPHEREKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DilateSphereKernel : public Operation
{
  public:
    explicit DilateSphereKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEDILATESPHEREKERNEL_HPP
