
#ifndef __TIER1_CLEREPLACEINTENSITYKERNEL_HPP
#define __TIER1_CLEREPLACEINTENSITYKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ReplaceIntensityKernel : public Operation
{
public:
  explicit ReplaceIntensityKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetInValue(const float & value) -> void;
  auto
  SetOutValue(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLEREPLACEINTENSITYKERNEL_HPP
