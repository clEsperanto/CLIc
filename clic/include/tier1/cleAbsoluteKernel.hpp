#ifndef __TIER1_CLEABSOLUTEKERNEL_HPP
#define __TIER1_CLEABSOLUTEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AbsoluteKernel : public Operation
{
public:
  explicit AbsoluteKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;
};

inline auto
AbsoluteKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  AbsoluteKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEABSOLUTEKERNEL_HPP
