#ifndef __TIER1_CLEBINARYEDGEDETECTIONKERNEL_HPP
#define __TIER1_CLEBINARYEDGEDETECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class BinaryEdgeDetectionKernel : public Operation
{
public:
  explicit BinaryEdgeDetectionKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

inline auto
BinaryEdgeDetectionKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  BinaryEdgeDetectionKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEBINARYEDGEDETECTIONKERNEL_HPP
