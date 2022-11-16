

#ifndef __TIER1_CLESUMREDUCTIONXKERNEL_HPP
#define __TIER1_CLESUMREDUCTIONXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumReductionXKernel : public Operation
{
public:
  explicit SumReductionXKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetBlocksize(const int & size) -> void;
};

inline auto
SumReductionXKernel_Call(const std::shared_ptr<cle::Processor> & device,
                         const Image &                           src,
                         const Image &                           dst,
                         const int &                             value) -> void
{
  SumReductionXKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetBlocksize(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESUMREDUCTIONXKERNEL_HPP
