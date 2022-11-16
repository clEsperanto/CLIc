
#ifndef __TIER1_CLEBINARYANDKERNEL_HPP
#define __TIER1_CLEBINARYANDKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryAndKernel : public Operation
{
public:
  explicit BinaryAndKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
BinaryAndKernel_Call(const std::shared_ptr<cle::Processor> & device,
                     const Image &                           src1,
                     const Image &                           src2,
                     const Image &                           dst) -> void
{
  BinaryAndKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEBINARYANDKERNEL_HPP
