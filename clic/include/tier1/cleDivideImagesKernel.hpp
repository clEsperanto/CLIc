
#ifndef __TIER1_CLEDIVIDEIMAGESKERNEL_HPP
#define __TIER1_CLEDIVIDEIMAGESKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DivideImagesKernel : public Operation
{
public:
  explicit DivideImagesKernel(const ProcessorPointer & device);
  auto
  SetInput1(const Image & object) -> void;
  auto
  SetInput2(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLEDIVIDEIMAGESKERNEL_HPP
