
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

inline auto
SmallerConstantKernel_Call(const std::shared_ptr<cle::Processor> & device,
                           const Image &                           src,
                           const Image &                           dst,
                           const float &                           value) -> void
{
  SmallerConstantKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetConstant(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESMALLERCONSTANTKERNEL_HPP
