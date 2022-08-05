
#ifndef __TIER1_CLECOPYKERNEL_HPP
#define __TIER1_CLECOPYKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class CopyKernel : public Operation
{
public:
  explicit CopyKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
};

} // namespace cle

#endif // __TIER1_CLECOPYKERNEL_HPP
