

#ifndef __TIER1_CLENONZEROMINIMUMBOXKERNEL_HPP
#define __TIER1_CLENONZEROMINIMUMBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class NonzeroMinimumBoxKernel : public Operation
{
public:
  explicit NonzeroMinimumBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetOutputFlag(const Image & object) -> void;
};

inline auto
NonzeroMinimumBoxKernel_Call(const std::shared_ptr<cle::Processor> & device,
                             const Image &                           src,
                             const Image &                           dst,
                             const Image &                           flag) -> void
{
  NonzeroMinimumBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetOutputFlag(flag);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLENONZEROMINIMUMBOXKERNEL_HPP
