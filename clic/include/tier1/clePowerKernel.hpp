
#ifndef __TIER1_CLEPOWERKERNEL_HPP
#define __TIER1_CLEPOWERKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class PowerKernel : public Operation
{
public:
  explicit PowerKernel(const ProcessorPointer & device);

  auto
  SetInput(const Image & object) -> void;

  auto
  SetOutput(const Image & object) -> void;

  auto
  SetExponent(const float & value) -> void;
};

inline auto
PowerKernel_Call(const std::shared_ptr<cle::Processor> & device,
                 const Image &                           src,
                 const Image &                           dst,
                 const float &                           scalar) -> void
{
  PowerKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetExponent(scalar);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEPOWERKERNEL_HPP
