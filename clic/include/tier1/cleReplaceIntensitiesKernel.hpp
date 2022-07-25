

#ifndef CLIC_INCLUDE_TIER1_CLEREPLACEINTENSITIESKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEREPLACEINTENSITIESKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ReplaceIntensitiesKernel : public Operation
{
  public:
    explicit ReplaceIntensitiesKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetMap (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEREPLACEINTENSITIESKERNEL_HPP
