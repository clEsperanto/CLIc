
#ifndef __TIER3_CLEHISTOGRAMKERNEL_HPP
#define __TIER3_CLEHISTOGRAMKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class HistogramKernel : public Operation
{
public:
  explicit HistogramKernel(const ProcessorPointer & device);
  auto
  SetInput(const Image & object) -> void;
  auto
  SetOutput(const Image & object) -> void;
  auto
  SetMinimumIntensity(const float & value) -> void;
  auto
  SetMaximumIntensity(const float & value) -> void;
  auto
  SetSteps(const int & step_x, const int & step_y, const int & step_z) -> void;
  auto
  SetNumBins(const size_t & bin) -> void;
  auto
  Execute() -> void override;

private:
  float  min_intensity_ = std::numeric_limits<float>::infinity();
  float  max_intensity_ = std::numeric_limits<float>::infinity();
  size_t nb_bins_ = 256;
};

inline auto
HistogramKernel_Call(const std::shared_ptr<cle::Processor> & device,
                     const Image &                           src,
                     const Image &                           dst,
                     const float &                           min_value,
                     const float &                           max_value,
                     const size_t &                          bin) -> void
{
  HistogramKernel kernel(device);
  kernel.SetInput(src);
  kernel.SetOutput(dst);
  kernel.SetMinimumIntensity(min_value);
  kernel.SetMaximumIntensity(max_value);
  kernel.SetSteps(1, 1, 1);
  kernel.SetNumBins(bin);
  kernel.Execute();
}

} // namespace cle

#endif // __TIER3_CLEHISTOGRAMKERNEL_HPP
