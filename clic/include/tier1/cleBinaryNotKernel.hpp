
#ifndef __TIER1_CLEBINARYNOTKERNEL_HPP
#define __TIER1_CLEBINARYNOTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryNotKernel : public Operation
{
public:
  explicit BinaryNotKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
BinaryNotKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  BinaryNotKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEBINARYNOTKERNEL_HPP
