#ifndef __TIER1_CLEDILATEBOXKERNEL_HPP
#define __TIER1_CLEDILATEBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DilateBoxKernel : public Operation
{
public:
  explicit DilateBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
DilateBoxKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  DilateBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEDILATEBOXKERNEL_HPP
