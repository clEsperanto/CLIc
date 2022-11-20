#ifndef __TIER1_CLEGREATEROREQUALKERNEL_HPP
#define __TIER1_CLEGREATEROREQUALKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterOrEqualKernel : public Operation
{
public:
  explicit GreaterOrEqualKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
GreaterOrEqualKernel_Call(const std::shared_ptr<cle::Processor> & device,
                          const Image &                           src1,
                          const Image &                           src2,
                          const Image &                           dst) -> void
{
  GreaterOrEqualKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEGREATEROREQUALKERNEL_HPP
