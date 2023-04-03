

#ifndef __TIER1_CLELAPLACEBOXKERNEL_HPP
#define __TIER1_CLELAPLACEBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class LaplaceBoxKernel : public Operation
{
public:
  explicit LaplaceBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
LaplaceBoxKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  LaplaceBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLELAPLACEBOXKERNEL_HPP