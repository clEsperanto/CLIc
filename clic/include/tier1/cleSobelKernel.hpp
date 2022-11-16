
#ifndef __TIER1_CLESOBELKERNEL_HPP
#define __TIER1_CLESOBELKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SobelKernel : public Operation
{
public:
  explicit SobelKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
SobelKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  SobelKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESOBELKERNEL_HPP
