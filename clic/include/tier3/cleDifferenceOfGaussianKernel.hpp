
#ifndef __TIER3_CLEDIFFERENCEOFGAUSSIANKERNEL_HPP
#define __TIER3_CLEDIFFERENCEOFGAUSSIANKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DifferenceOfGaussianKernel : public Operation
{
public:
  explicit DifferenceOfGaussianKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetSigma1(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void;
  auto
  SetSigma2(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void;
  auto
  Execute() -> void override;

private:
  std::array<float, 3> sigma1_ = { 1, 1, 1 };
  std::array<float, 3> sigma2_ = { 2, 2, 2 };
};

} // namespace cle

#endif // __TIER3_CLEDIFFERENCEOFGAUSSIANKERNEL_HPP
