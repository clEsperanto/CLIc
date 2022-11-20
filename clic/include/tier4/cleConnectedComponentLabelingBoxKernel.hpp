
#ifndef __TIER4_CLECONNECTEDCOMPONENTLABELINGBOXKERNEL_HPP
#define __TIER4_CLECONNECTEDCOMPONENTLABELINGBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ConnectedComponentLabelingBoxKernel : public Operation
{
public:
  explicit ConnectedComponentLabelingBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
ConnectedComponentLabelingBoxKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                         const Image &                           src,
                                         const Image &                           dst) -> void
{
  ConnectedComponentLabelingBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER4_CLECONNECTEDCOMPONENTLABELINGBOXKERNEL_HPP
