#ifndef __TIER1_CLEGAUSSIANBLURKERNEL_HPP
#define __TIER1_CLEGAUSSIANBLURKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GaussianBlurKernel : public Operation
{
public:
  explicit GaussianBlurKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetSigma(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void;
  auto
  Execute() -> void override;

private:
  std::array<float, 3> sigma_{ 0, 0, 0 };
  static auto
  Sigma2KernelSize(const std::array<float, 3> & sigmas) -> std::array<size_t, 3>;
};

inline auto
GaussianBlurKernel_Call(const std::shared_ptr<cle::Processor> & device,
                        const Image &                           src,
                        const Image &                           dst,
                        const float &                           sigma_x,
                        const float &                           sigma_y,
                        const float &                           sigma_z) -> void
{
  GaussianBlurKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetSigma(sigma_x, sigma_y, sigma_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEGAUSSIANBLURKERNEL_HPP
