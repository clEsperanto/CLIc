
#ifndef __TIER1_CLEERODESPHEREKERNEL_HPP
#define __TIER1_CLEERODESPHEREKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ErodeSphereKernel : public Operation
{
public:
  explicit ErodeSphereKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
ErodeSphereKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  ErodeSphereKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEERODESPHEREKERNEL_HPP
