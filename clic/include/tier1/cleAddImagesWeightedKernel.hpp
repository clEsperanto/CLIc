
#ifndef __TIER1_CLEADDIMAGESWEIGHTEDKERNEL_HPP
#define __TIER1_CLEADDIMAGESWEIGHTEDKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AddImagesWeightedKernel : public Operation
{
public:
  explicit AddImagesWeightedKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetFactor1(const float & value) -> void;
  auto
  SetFactor2(const float & value) -> void;
};

inline auto
AddImagesWeightedKernel_Call(const std::shared_ptr<cle::Processor> & device,
                             const Image &                           src1,
                             const Image &                           src2,
                             const Image &                           dst,
                             const float &                           w1,
                             const float &                           w2) -> void
{
  AddImagesWeightedKernel kernel(device);
  kernel.SetInput1(src1);
  kernel.SetInput2(src2);
  kernel.SetOutput(dst);
  kernel.SetFactor1(w1);
  kernel.SetFactor2(w2);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEADDIMAGESWEIGHTEDKERNEL_HPP
