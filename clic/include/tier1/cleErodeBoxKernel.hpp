#ifndef __TIER1_CLEERODEBOXKERNEL_HPP
#define __TIER1_CLEERODEBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ErodeBoxKernel : public Operation
{
public:
  explicit ErodeBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
ErodeBoxKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  ErodeBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEERODEBOXKERNEL_HPP
