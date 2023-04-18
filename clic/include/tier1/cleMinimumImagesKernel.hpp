#ifndef __TIER1_CLEMINIMUMIMAGESKERNEL_HPP
#define __TIER1_CLEMINIMUMIMAGESKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MinimumImagesKernel : public Operation
{
public:
  explicit MinimumImagesKernel(const ProcessorPointer & device);
  void
  SetInput1(const Image & object);
  void
  SetInput2(const Image & object);
  void
  SetOutput(const Image & object);
};

inline void
MinimumImagesKernel_Call(const std::shared_ptr<cle::Processor> & device,
                         const Image &                           src1,
                         const Image &                           src2,
                         const Image &                           dst)
{
  MinimumImagesKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEMINIMUMIMAGESKERNEL_HPP
