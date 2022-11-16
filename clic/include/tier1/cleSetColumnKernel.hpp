

#ifndef __TIER1_CLESETCOLUMNKERNEL_HPP
#define __TIER1_CLESETCOLUMNKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SetColumnKernel : public Operation
{
public:
  explicit SetColumnKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetColumn(const int & index) -> void;
  auto
  SetValue(const float & value) -> void;
};

inline auto
SetColumnKernel_Call(const std::shared_ptr<cle::Processor> & device,
                     const Image &                           src,
                     const int &                             index,
                     const float &                           value) -> void
{
  SetColumnKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetColumn(index);
  kernel.SetValue(value);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLESETCOLUMNKERNEL_HPP
