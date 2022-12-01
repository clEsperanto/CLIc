#ifndef __TIER1_CLECONVOLVEKERNEL_HPP
#define __TIER1_CLECONVOLVEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ConvolveKernel : public Operation
{
public:
  explicit ConvolveKernel(const ProcessorPointer & device);

  auto
  SetInput1(const Image & object) -> void;

  auto
  SetInput2(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;
};

inline auto
ConvolveKernel_Call(const std::shared_ptr<cle::Processor> & device,
                    const Image &                           src,
                    const Image &                           convolve_kernel,
                    const Image &                           dst) -> void
{
  ConvolveKernel kernel(device);
  kernel.SetInput1(src);
  kernel.SetInput2(convolve_kernel);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLECONVOLVEKERNEL_HPP
