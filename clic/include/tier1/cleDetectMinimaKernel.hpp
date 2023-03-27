
#ifndef __TIER1_CLEDETECTMINIMAKERNEL_HPP
#define __TIER1_CLEDETECTMINIMAKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{
    
class DetectMinimaKernel : public Operation
{
public:
  explicit DetectMinimaKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;
};

inline auto
DetectMinimaKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  DetectMinimaKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}
} // namespace cle

#endif // __TIER1_CLEDETECTMINIMAKERNEL_HPP