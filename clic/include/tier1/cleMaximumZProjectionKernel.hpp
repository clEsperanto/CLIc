#ifndef __TIER1_CLEMAXIMUMZPROJECTIONKERNEL_HPP
#define __TIER1_CLEMAXIMUMZPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumZProjectionKernel : public Operation
{
public:
  explicit MaximumZProjectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
MaximumZProjectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  MaximumZProjectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMAXIMUMZPROJECTIONKERNEL_HPP
