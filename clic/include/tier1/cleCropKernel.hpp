
#ifndef __TIER1_CLECROPKERNEL_HPP
#define __TIER1_CLECROPKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class CropKernel : public Operation
{
public:
  explicit CropKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;

  auto
  SetIndex(const int & index0, const int & index1, const int & index2) -> void;
};

inline auto
CropKernel_Call(const std::shared_ptr<cle::Processor> & device,
                const Image &                           src,
                const Image &                           dst,
                const int &                             index0,
                const int &                             index1,
                const int &                             index2) -> void
{
  CropKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetIndex(index0, index1, index2);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLECROPKERNEL_HPP