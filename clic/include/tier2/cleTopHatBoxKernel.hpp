
#ifndef __TIER2_CLETOPHATBOXKERNEL_HPP
#define __TIER2_CLETOPHATBOXKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class TopHatBoxKernel : public Operation
{
public:
  explicit TopHatBoxKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetRadius(const int & radius_x = 0, const int & radius_y = 0, const int & radius_z = 0) -> void;
  auto
  Execute() -> void override;

private:
  std::array<int, 3> radius_{ 0, 0, 0 };
};

inline auto
TopHatBoxKernel_Call(const std::shared_ptr<cle::Processor> & device,
                     const Image &                           src,
                     const Image &                           dst,
                     const int &                             radius_x,
                     const int &                             radius_y,
                     const int &                             radius_z) -> void
{
  TopHatBoxKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetRadius(radius_x, radius_y, radius_z);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER2_CLETOPHATBOXKERNEL_HPP
