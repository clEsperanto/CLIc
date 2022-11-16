
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

inline auto
DifferenceOfGaussianKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                const Image &                           src,
                                const Image &                           dst,
                                const float &                           sigma1_x,
                                const float &                           sigma1_y,
                                const float &                           sigma1_z,
                                const float &                           sigma2_x,
                                const float &                           sigma2_y,
                                const float &                           sigma2_z) -> void
{
  DifferenceOfGaussianKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetSigma1(sigma1_x, sigma1_y, sigma1_z);
  kernel.SetSigma2(sigma2_x, sigma2_y, sigma2_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER3_CLEDIFFERENCEOFGAUSSIANKERNEL_HPP
