
#ifndef __TIER1_CLEONLYZEROOVERWRITEMAXIMUMDIAMONDKERNEL_HPP
#define __TIER1_CLEONLYZEROOVERWRITEMAXIMUMDIAMONDKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class OnlyzeroOverwriteMaximumDiamondKernel : public Operation
{
public:
  explicit OnlyzeroOverwriteMaximumDiamondKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput1(const Image & object) -> void;
  auto
  SetOutput2(const Image & object) -> void;
};

inline auto
OnlyzeroOverwriteMaximumDiamondKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                           const Image &                           src,
                                           const Image &                           dst1,
                                           const Image &                           dst2) -> void
{
  OnlyzeroOverwriteMaximumDiamondKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput1(dst1);
  kernel.SetOutput2(dst2);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEONLYZEROOVERWRITEMAXIMUMDIAMONDKERNEL_HPP
