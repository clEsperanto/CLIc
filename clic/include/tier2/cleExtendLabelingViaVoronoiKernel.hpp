#ifndef __TIER2_CLEEXTENDLABELINGVIAVORONOIKERNEL_HPP
#define __TIER2_CLEEXTENDLABELINGVIAVORONOIKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ExtendLabelingViaVoronoiKernel : public Operation
{
public:
  explicit ExtendLabelingViaVoronoiKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
ExtendLabelingViaVoronoiKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                    const Image &                           src,
                                    const Image &                           dst) -> void
{
  ExtendLabelingViaVoronoiKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER2_CLEEXTENDLABELINGVIAVORONOIKERNEL_HPP
