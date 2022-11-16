

#ifndef __TIER1_CLEREPLACEINTENSITIESKERNEL_HPP
#define __TIER1_CLEREPLACEINTENSITIESKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class ReplaceIntensitiesKernel : public Operation
{
public:
  explicit ReplaceIntensitiesKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetMap(const Image & object) -> void;
};

inline auto
ReplaceIntensitiesKernel_Call(const std::shared_ptr<cle::Processor> & device,
                              const Image &                           src,
                              const Image &                           dst,
                              const Image &                           map) -> void
{
  ReplaceIntensitiesKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetMap(map);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEREPLACEINTENSITIESKERNEL_HPP
