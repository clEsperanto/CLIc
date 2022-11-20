
#ifndef __TIER1_CLEREPLACEINTENSITYKERNEL_HPP
#define __TIER1_CLEREPLACEINTENSITYKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ReplaceIntensityKernel : public Operation
{
public:
  explicit ReplaceIntensityKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetInValue(const float & value) -> void;
  auto
  SetOutValue(const float & value) -> void;
};

inline auto
ReplaceIntensityKernel_Call(const std::shared_ptr<cle::Processor> & device,
                            const Image &                           src,
                            const Image &                           dst,
                            const float &                           in_value,
                            const float &                           out_value) -> void
{
  ReplaceIntensityKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetInValue(in_value);
  kernel.SetInValue(out_value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEREPLACEINTENSITYKERNEL_HPP
