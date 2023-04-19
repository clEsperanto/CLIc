#ifndef __TIER1_CLEDIVIDEIMAGEANDSCALARKERNEL_HPP
#define __TIER1_CLEDIVIDEIMAGEANDSCALARKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DivideImageAndScalarKernel : public Operation
{
public:
  explicit DivideImageAndScalarKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & scalar) -> void;
};

inline auto
DivideImageAndScalarKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                const Image &                           src,
                                const Image &                           dst,
                                const float &                           scalar) -> void
{
  DivideImageAndScalarKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetScalar(scalar);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEDIVIDEIMAGEANDSCALARKERNEL_HPP
