
#ifndef __TIER1_CLECOPYKERNEL_HPP
#define __TIER1_CLECOPYKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class CopyKernel : public Operation
{
public:
  explicit CopyKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
CopyKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  CopyKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLECOPYKERNEL_HPP
