
#ifndef __TIER1_CLEMINIMUMBOXKERNEL_HPP
#define __TIER1_CLEMINIMUMBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumBoxKernel : public Operation
{
public:
  explicit MinimumBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetRadius(const int & radius_x = 0, const int & radius_y = 0, const int & radius_z = 0) -> void;
  auto
  Execute() -> void override;

private:
  std::array<int, 3> radius_{ 0, 0, 0 };
  [[nodiscard]] auto
  Radius2KernelSize() const -> std::array<int, 3>;
};

inline auto
MinimumBoxKernel_Call(const std::shared_ptr<cle::Processor> & device,
                      const Image &                           src,
                      const Image &                           dst,
                      const float &                           radius_x,
                      const float &                           radius_y,
                      const float &                           radius_z) -> void
{
  MinimumBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMINIMUMBOXKERNEL_HPP
