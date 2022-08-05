
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
  auto
  Radius2KernelSize() const -> std::array<int, 3>;
};

} // namespace cle

#endif // __TIER1_CLEMAXIMUMBOXKERNEL_HPP
