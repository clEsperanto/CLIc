

#ifndef __TIER1_CLENOTEQUALCONSTANTKERNEL_HPP
#define __TIER1_CLENOTEQUALCONSTANTKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class NotEqualConstantKernel : public Operation
{
public:
  explicit NotEqualConstantKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetConstant(const float & value) -> void;
};

inline auto
NotEqualConstantKernel_Call(const std::shared_ptr<cle::Processor> & device,
                            const Image &                           src,
                            const Image &                           dst,
                            const float &                           value) -> void
{
  NotEqualConstantKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetConstant(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLENOTEQUALCONSTANTKERNEL_HPP
