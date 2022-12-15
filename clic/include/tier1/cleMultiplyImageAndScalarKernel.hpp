
#ifndef __TIER1_CLEMULTIPLYIMAGEANDSCALARKERNEL_HPP
#define __TIER1_CLEMULTIPLYIMAGEANDSCALARKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MultiplyImageAndScalarKernel : public Operation
{
public:
  explicit MultiplyImageAndScalarKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

inline auto
MultiplyImageAndScalarKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                  const Image &                           src,
                                  const Image &                           dst,
                                  const float &                           value) -> void
{
  MultiplyImageAndScalarKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetScalar(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMULTIPLYIMAGEANDSCALARKERNEL_HPP
