
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

} // namespace cle

#endif // __TIER1_CLEFLAGEXISTINGLABELSKERNEL_HPP
