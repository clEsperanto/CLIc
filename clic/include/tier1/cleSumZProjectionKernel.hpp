

#ifndef __TIER1_CLESUMZPROJECTIONKERNEL_HPP
#define __TIER1_CLESUMZPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumZProjectionKernel : public Operation
{
public:
  explicit SumZProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
SumZProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  SumZProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESUMZPROJECTIONKERNEL_HPP
