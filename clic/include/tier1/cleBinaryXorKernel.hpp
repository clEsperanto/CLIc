
#ifndef __TIER1_CLEBINARYXORKERNEL_HPP
#define __TIER1_CLEBINARYXORKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryXorKernel : public Operation
{
public:
  explicit BinaryXorKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
BinaryXorKernel_Call(const std::shared_ptr<cle::Processor> & device,
                     const Image &                           src1,
                     const Image &                           src2,
                     const Image &                           dst) -> void
{
  BinaryXorKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEBINARYXORKERNEL_HPP
