

#ifndef CLIC_INCLUDE_TIER1_CLESUMREDUCTIONXKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLESUMREDUCTIONXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumReductionXKernel : public Operation
{
  public:
    explicit SumReductionXKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetBlocksize (const int &value) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLESUMREDUCTIONXKERNEL_HPP
