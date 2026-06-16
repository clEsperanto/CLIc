#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"
#include "tier8.hpp"

#include "utils.hpp"

namespace cle::tier8
{

auto
make_isotropic_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   current_spacing_x,
                    float                   current_spacing_y,
                    float                   current_spacing_z,
                    float                   target_spacing,
                    bool                    interpolate) -> Array::Pointer
{

  if (current_spacing_x <= 0 || current_spacing_y <= 0 || current_spacing_z <= 0)
  {
    throw std::runtime_error(
      "Error: current spacings must be positive values. Provided values are current_spacing_x=" + std::to_string(current_spacing_x) +
      ", current_spacing_y=" + std::to_string(current_spacing_y) + ", current_spacing_z=" + std::to_string(current_spacing_z));
  }
  if (target_spacing <= 0)
  {
    target_spacing = std::min({ current_spacing_x, current_spacing_y, current_spacing_z });
  }

  auto rescaling_factor_x = current_spacing_x / target_spacing;
  auto rescaling_factor_y = current_spacing_y / target_spacing;
  auto rescaling_factor_z = current_spacing_z / target_spacing;

  auto result = tier7::scale_func(device, src, dst, rescaling_factor_x, rescaling_factor_y, rescaling_factor_z, true, interpolate, true);

  return result;
}

auto
make_anisotropic_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      float                   current_spacing,
                      float                   target_spacing_x,
                      float                   target_spacing_y,
                      float                   target_spacing_z,
                      bool                    interpolate) -> Array::Pointer
{

  if (target_spacing_x <= 0 || target_spacing_y <= 0 || target_spacing_z <= 0)
  {
    throw std::runtime_error(
      "Error: target spacings must be positive values. Provided values are target_spacing_x=" + std::to_string(target_spacing_x) +
      ", target_spacing_y=" + std::to_string(target_spacing_y) + ", target_spacing_z=" + std::to_string(target_spacing_z));
  }
  if (current_spacing <= 0)
  {
    current_spacing = std::min({ target_spacing_x, target_spacing_y, target_spacing_z });
  }

  auto rescaling_factor_x = current_spacing / target_spacing_x;
  auto rescaling_factor_y = current_spacing / target_spacing_y;
  auto rescaling_factor_z = current_spacing / target_spacing_z;

  auto result = tier7::scale_func(device, src, dst, rescaling_factor_x, rescaling_factor_y, rescaling_factor_z, true, interpolate, true);

  return result;
}


} // namespace cle::tier8
