
#ifndef __TIER1_CLEONLYZEROOVERWRITEMAXIMUMBOXKERNEL_HPP
#define __TIER1_CLEONLYZEROOVERWRITEMAXIMUMBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class OnlyzeroOverwriteMaximumBoxKernel : public Operation
{
public:
  explicit OnlyzeroOverwriteMaximumBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput1(const Image & object) -> void;
  auto
  SetOutput2(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEONLYZEROOVERWRITEMAXIMUMBOXKERNEL_HPP
