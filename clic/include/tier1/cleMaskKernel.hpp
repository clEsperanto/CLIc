#ifndef __TIER1_CLEMASKKERNEL_HPP
#define __TIER1_CLEMASKKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaskKernel : public Operation
{
public:
  explicit MaskKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetMask(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
MaskKernel_Call(const std::shared_ptr<cle::Processor> & device,
                const Image &                           src,
                const Image &                           mask,
                const Image &                           dst) -> void
{
  MaskKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetMask(mask);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMASKKERNEL_HPP
