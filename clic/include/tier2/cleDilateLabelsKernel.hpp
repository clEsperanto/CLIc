
#ifndef __TIER2_CLEDILATELABELSKERNEL_HPP
#define __TIER2_CLEDILATELABELSKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DilateLabelsKernel : public Operation
{
public:
  explicit DilateLabelsKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetRadius(const int & radius) -> void;
  auto
  Execute() -> void override;

private:
  int radius_ = 1;
};

} // namespace cle

#endif // __TIER2_CLEDILATELABELSKERNEL_HPP
