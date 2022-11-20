
#ifndef __TIER1_CLESETKERNEL_HPP
#define __TIER1_CLESETKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SetKernel : public Operation
{
public:
  explicit SetKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetValue(const float & value) -> void;
};

inline auto
SetKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const float & value) -> void
{
  SetKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetValue(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESETKERNEL_HPP
