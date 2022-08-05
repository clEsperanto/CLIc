#ifndef __TIER1_CLEEXECUTESEPARABLEKERNEL_HPP
#define __TIER1_CLEEXECUTESEPARABLEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ExecuteSeparableKernel : public Operation
{
public:
  explicit ExecuteSeparableKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetSigma(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void;
  auto
  SetKernelSize(const int & radius_x, const int & radius_y, const int & radius_z) -> void;
  auto
  Execute() -> void override;

private:
  int                  dim_ = 1;
  std::array<float, 3> sigma_{ 0, 0, 0 };
  std::array<int, 3>   kernel_size_{ 0, 0, 0 };
};

} // namespace cle

#endif // __TIER1_CLEEXECUTESEPARABLEKERNEL_HPP
