
#ifndef __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP
#define __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class AddImageAndScalarKernel : public Operation
{
public:
  explicit AddImageAndScalarKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetScalar(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP
