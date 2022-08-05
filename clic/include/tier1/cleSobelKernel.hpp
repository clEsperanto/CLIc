
#ifndef __TIER1_CLESOBELKERNEL_HPP
#define __TIER1_CLESOBELKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SobelKernel : public Operation
{
public:
  explicit SobelKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLESOBELKERNEL_HPP
