
#ifndef __TIER1_CLEFLAGEXISTINGLABELSKERNEL_HPP
#define __TIER1_CLEFLAGEXISTINGLABELSKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class FlagExistingLabelsKernel : public Operation
{
public:
  explicit FlagExistingLabelsKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  Execute() -> void override;
};

inline auto
FlagExistingLabelsKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst)
  -> void
{
  FlagExistingLabelsKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER1_CLEFLAGEXISTINGLABELSKERNEL_HPP
