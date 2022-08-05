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

} // namespace cle

#endif // __TIER2_CLEEXTENDLABELINGVIAVORONOIKERNEL_HPP
