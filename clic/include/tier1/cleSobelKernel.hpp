
#ifndef CLIC_INCLUDE_TIER1_CLESOBELKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLESOBELKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SobelKernel : public Operation
{
  public:
    explicit SobelKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLESOBELKERNEL_HPP
