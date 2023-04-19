#ifndef __TIER1_CLEMAXIMUMIMAGESKERNEL_HPP
#define __TIER1_CLEMAXIMUMIMAGESKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumImagesKernel : public Operation
{
public:
  explicit MaximumImagesKernel(const ProcessorPointer & device);
  void
  SetInput1(const Image & object);
  void
  SetInput2(const Image & object);
  void
  SetOutput(const Image & object);
};

inline auto
MaximumImagesKernel_Call(const std::shared_ptr<cle::Processor> & device,
                         const Image &                           src1,
                         const Image &                           src2,
                         const Image &                           dst) -> void
{
  MaximumImagesKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMAXIMUMIMAGESKERNEL_HPP
