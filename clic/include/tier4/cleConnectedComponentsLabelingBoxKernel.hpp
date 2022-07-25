
#ifndef CLIC_INCLUDE_TIER4_CLECONNECTEDCOMPONENTSLABELINGBOXKERNEL_HPP
#define CLIC_INCLUDE_TIER4_CLECONNECTEDCOMPONENTSLABELINGBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ConnectedComponentsLabelingBoxKernel : public Operation
{
  public:
    explicit ConnectedComponentsLabelingBoxKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto Execute () -> void override;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER4_CLECONNECTEDCOMPONENTSLABELINGBOXKERNEL_HPP
