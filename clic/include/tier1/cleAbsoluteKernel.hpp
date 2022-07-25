#ifndef CLIC_INCLUDE_TIER1_CLEABSOLUTEKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEABSOLUTEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AbsoluteKernel : public Operation
{
  public:
    explicit AbsoluteKernel (const ProcessorPointer &device);
    ~AbsoluteKernel () = default;
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto GetOutput () -> Image;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEABSOLUTEKERNEL_HPP
