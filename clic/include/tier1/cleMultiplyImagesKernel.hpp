
#ifndef __TIER1_CLEMULTIPLYIMAGESKERNEL_HPP
#define __TIER1_CLEMULTIPLYIMAGESKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MultiplyImagesKernel : public Operation
{
public:
  explicit MultiplyImagesKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEMULTIPLYIMAGESKERNEL_HPP
