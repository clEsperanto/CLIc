
#ifndef __TIER1_CLESMALLERCONSTANTKERNEL_HPP
#define __TIER1_CLESMALLERCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SmallerConstantKernel : public Operation
{
public:
  explicit SmallerConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetConstant(const float & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLESMALLERCONSTANTKERNEL_HPP
