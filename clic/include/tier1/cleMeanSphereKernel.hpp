
#ifndef __TIER1_CLEMEANSPHEREKERNEL_HPP
#define __TIER1_CLEMEANSPHEREKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MeanSphereKernel : public Operation
{
public:
  explicit MeanSphereKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetRadius(const float & radius_x, const float & radius_y, const float & radius_z) -> void;

private:
  [[nodiscard]] auto
  Radius2KernelSize(const float & radius) const -> int;
};

inline auto
MeanSphereKernel_Call(const std::shared_ptr<cle::Processor> & device,
                      const Image &                           src,
                      const Image &                           dst,
                      const float &                           radius_x,
                      const float &                           radius_y,
                      const float &                           radius_z) -> void
{
  MeanSphereKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMEANSPHEREKERNEL_HPP
