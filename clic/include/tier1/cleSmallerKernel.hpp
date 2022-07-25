
#ifndef CLIC_INCLUDE_TIER1_CLESMALLERKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLESMALLERKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SmallerKernel : public Operation
{
  public:
    explicit SmallerKernel (const ProcessorPointer &device);
    auto SetInput1 (const Image &object) -> void;
    auto SetInput2 (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLESMALLERKERNEL_HPP
