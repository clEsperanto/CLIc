
#ifndef __TIER1_CLEGREATEROREQUALCONSTANTKERNEL_HPP
#define __TIER1_CLEGREATEROREQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class GreaterOrEqualConstantKernel : public Operation
{
public:
  explicit GreaterOrEqualConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetConstant(const float & value) -> void;
};

inline auto
GreaterOrEqualConstantKernel_Call(const std::shared_ptr<cle::Processor> & device,
                                  const Image &                           src,
                                  const Image &                           dst,
                                  const float &                           value) -> void
{
  GreaterOrEqualConstantKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetConstant(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEGREATEROREQUALCONSTANTKERNEL_HPP
