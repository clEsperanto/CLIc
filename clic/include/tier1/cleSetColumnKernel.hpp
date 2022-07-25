

#ifndef CLIC_INCLUDE_TIER1_CLESETCOLUMNKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLESETCOLUMNKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SetColumnKernel : public Operation
{
  public:
    SetColumnKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetColumn (const int &value) -> void;
    auto SetValue (const float &value) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLESETCOLUMNKERNEL_HPP
