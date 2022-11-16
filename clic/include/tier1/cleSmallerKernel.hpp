
#ifndef __TIER1_CLESMALLERKERNEL_HPP
#define __TIER1_CLESMALLERKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SmallerKernel : public Operation
{
public:
  explicit SmallerKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
SmallerKernel_Call(const std::shared_ptr<cle::Processor> & device,
                   const Image &                           src1,
                   const Image &                           src2,
                   const Image &                           dst) -> void
{
  SmallerKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESMALLERKERNEL_HPP
