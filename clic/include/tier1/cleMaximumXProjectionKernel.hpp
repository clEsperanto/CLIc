#ifndef CLIC_INCLUDE_TIER1_CLEMAXIMUMXPROJECTIONKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEMAXIMUMXPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumXProjectionKernel : public Operation
{
  public:
    explicit MaximumXProjectionKernel (const ProcessorPointer &device);
    ~MaximumXProjectionKernel () = default;

    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto GetOutput () -> Image;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEMAXIMUMXPROJECTIONKERNEL_HPP
