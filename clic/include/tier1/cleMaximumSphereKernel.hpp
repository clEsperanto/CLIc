#ifndef __TIER1_CLEMAXIMUMSPHEREKERNEL_HPP
#define __TIER1_CLEMAXIMUMSPHEREKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumSphereKernel : public Operation
{
public:
  explicit MaximumSphereKernel(const ProcessorPointer & device);

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
MaximumSphereKernel_Call(const std::shared_ptr<cle::Processor> & device,
                         const Image &                           src,
                         const Image &                           dst,
                         const float &                           radius_x,
                         const float &                           radius_y,
                         const float &                           radius_z) -> void
{
  MaximumSphereKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMAXIMUMSPHEREKERNEL_HPP
