
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

inline auto
SetNonzeroPixelsToPixelindexKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                        const Image &                           src,
                                        const Image &                           dst,
                                        const float &                           value) -> void
{
  SetNonzeroPixelsToPixelindexKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetOffset(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESETNONZEROPIXELSTOPIXELINDEXKERNEL_HPP
