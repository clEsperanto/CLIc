
#ifndef __TIER2_CLEDETECTMAXIMAKERNEL_HPP
#define __TIER2_CLEDETECTMAXIMAKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class DetectMaximaKernel : public Operation
{
public:
  explicit DetectMaximaKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  //   auto SetRadius (const int &radius_x = 0, const int &radius_y = 0, const int &radius_z = 0) -> void;
  //   auto Execute () -> void override;
  // private:
  //   std::array<int, 3> radius_{ 0, 0, 0 };
};

inline auto
DetectMaximaKernel_Call(const std::shared_ptr<cle::Processor> & device, const Image & src, const Image & dst) -> void
{
  DetectMaximaKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER2_CLEDETECTMAXIMAKERNEL_HPP
