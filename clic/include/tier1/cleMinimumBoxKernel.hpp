
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
  auto
  Radius2KernelSize() const -> std::array<int, 3>;
};

} // namespace cle

#endif // __TIER1_CLEMINIMUMBOXKERNEL_HPP
