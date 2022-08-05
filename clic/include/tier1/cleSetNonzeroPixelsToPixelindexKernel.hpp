
#ifndef __TIER1_CLESETNONZEROPIXELSTOPIXELINDEXKERNEL_HPP
#define __TIER1_CLESETNONZEROPIXELSTOPIXELINDEXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SetNonzeroPixelsToPixelindexKernel : public Operation
{
public:
  explicit SetNonzeroPixelsToPixelindexKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetOffset(const int & value) -> void;
};

} // namespace cle

#endif // __TIER1_CLESETNONZEROPIXELSTOPIXELINDEXKERNEL_HPP
