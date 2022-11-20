
#ifndef __TIER1_CLEBLOCKENUMERATEKERNEL_HPP
#define __TIER1_CLEBLOCKENUMERATEKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

// todo: make kernel work with Image

class BlockEnumerateKernel : public Operation
{

public:
  explicit BlockEnumerateKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetInputSums(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetBlocksize(const int & value) -> void;
};

inline auto
BlockEnumerateKernel_Call(const std::shared_ptr<cle::Processor> & device,
                          const Image &                           src,
                          const Image &                           sum,
                          const Image &                           dst,
                          const int &                             value) -> void
{
  BlockEnumerateKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetInputSums(sum);
  kernel.SetOutput(dst);
  kernel.SetBlocksize(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEBLOCKENUMERATEKERNEL_HPP
