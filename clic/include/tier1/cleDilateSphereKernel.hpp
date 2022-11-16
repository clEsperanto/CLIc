
#ifndef __TIER1_CLEDILATESPHEREKERNEL_HPP
#define __TIER1_CLEDILATESPHEREKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DilateSphereKernel : public Operation
{
public:
  explicit DilateSphereKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
DilateSphereKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  DilateSphereKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEDILATESPHEREKERNEL_HPP
