
#ifndef __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP
#define __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AddImageAndScalarKernel : public Operation
{
public:
  explicit AddImageAndScalarKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

inline auto
AddImageAndScalarKernel_Call(const std::shared_ptr<cle::Processor> & device,
                             const Image &                           src,
                             const Image &                           dst,
                             const float &                           value) -> void
{
  AddImageAndScalarKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetScalar(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP
