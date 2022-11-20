
#ifndef __TIER1_CLEMAXIMUMBOXKERNEL_HPP
#define __TIER1_CLEMAXIMUMBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumBoxKernel : public Operation
{
public:
  explicit MaximumBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetRadius(const int & radius_x, const int & radius_y, const int & radius_z) -> void;
  auto
  Execute() -> void override;

private:
  std::array<int, 3> radius_{ 0, 0, 0 };
  [[nodiscard]] auto
  Radius2KernelSize() const -> std::array<int, 3>;
};

inline auto
MaximumBoxKernel_Call(const std::shared_ptr<cle::Processor> & device,
                      const Image &                           src,
                      const Image &                           dst,
                      const float &                           radius_x,
                      const float &                           radius_y,
                      const float &                           radius_z) -> void
{
  MaximumBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMAXIMUMBOXKERNEL_HPP
