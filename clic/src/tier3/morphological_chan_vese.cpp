#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
create_checkerboard_init(const Array::Pointer & src, Array::Pointer dst, int squareSize) -> void
{
  auto width = dst->width();
  auto height = dst->height();
  auto depth = dst->depth();

  std::vector<uint8_t> checkerboard(width * height * depth);
  for (int z = 0; z < depth; ++z)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        // Calculate the index for the 1D vector
        int index = z * width * height + y * width + x;
        // Determine the checkerboard pattern value
        checkerboard[index] = ((x / squareSize) + (y / squareSize) + (z / squareSize)) % 2;
      }
    }
  }
  dst->writeFrom(checkerboard.data());
}


auto
compute_contour_score(const Device::Pointer & device,
                      const Array::Pointer &  image,
                      const Array::Pointer &  contour,
                      float &                 c) -> void
{
  auto masked = tier1::mask_func(device, image, contour, nullptr);
  auto sum_image_value = tier2::sum_of_all_pixels_func(device, masked);
  auto sum_contour_value = tier2::sum_of_all_pixels_func(device, contour) + 1e-8;
  c = -sum_image_value / sum_contour_value;
}

auto
compute_gradient_magnitude(const Device::Pointer & device,
                           const Array::Pointer &  contour,
                           Array::Pointer          gradient_magnitude) -> void
{
  auto gradient_along_axis = Array::create(gradient_magnitude);
  auto absolute_gradient_along_axis = Array::create(gradient_magnitude);
  gradient_magnitude->fill(0);
  for (int d = 0; d < contour->dimension(); d++)
  {
    switch (d)
    {
      case 0:
        tier1::gradient_x_func(device, contour, gradient_along_axis);
        break;
      case 1:
        tier1::gradient_y_func(device, contour, gradient_along_axis);
        break;
      case 2:
        tier1::gradient_z_func(device, contour, gradient_along_axis);
        break;
    }
    tier1::absolute_func(device, gradient_along_axis, absolute_gradient_along_axis);
    tier1::add_images_weighted_func(device, absolute_gradient_along_axis, gradient_magnitude, gradient_magnitude, 1, 1);
  }
}

auto
compute_contour_evolution(const Device::Pointer & device,
                          const Array::Pointer &  image,
                          Array::Pointer          evolution,
                          float                   c0,
                          float                   c1,
                          float                   lambda1,
                          float                   lambda2) -> void
{
  // magnitude * (lambda1 * (image - c1) ** 2 - lambda2 * (image - c0) ** 2)
  // (image - c1) ** 2 -> inside_evo
  auto inside_evo = tier1::add_image_and_scalar_func(device, image, nullptr, c1);
  tier1::power_func(device, inside_evo, inside_evo, 2);
  // (image - c0) ** 2) -> outside_evo
  auto outside_evo = tier1::add_image_and_scalar_func(device, image, nullptr, c0);
  tier1::power_func(device, outside_evo, outside_evo, 2);
  // magnitude * (lambda1 * inside_evo - lambda2 * outside_evo)
  auto merged = tier1::add_images_weighted_func(device, inside_evo, outside_evo, nullptr, lambda1, -lambda2);
  tier1::multiply_images_func(device, merged, evolution, evolution);
}

auto
apply_contour_evolution(const Device::Pointer & device, const Array::Pointer & evolution, Array::Pointer contour)
  -> void
{
  auto evolution_pos = tier1::greater_constant_func(device, evolution, nullptr, 0);
  auto evolution_neg = tier1::smaller_constant_func(device, evolution, nullptr, 0);
  auto evolution_or = tier1::binary_or_func(device, evolution_pos, evolution_neg, nullptr);
  auto mask = tier1::binary_not_func(device, evolution_or, nullptr);
  auto masked_evolution = tier1::mask_func(device, contour, mask, nullptr);
  tier1::add_images_weighted_func(device, masked_evolution, evolution_neg, contour, 1, 1);
}

auto
smooth_contour(const Device::Pointer & device, Array::Pointer dst, int iteration) -> void
{
  if (iteration == 0)
  {
    return;
  }
  auto temp = Array::create(dst);
  for (int i = 0; i < iteration; i++)
  {
    if (i % 2 == 0)
    {
      tier1::binary_supinf_func(device, dst, temp);
      tier1::binary_infsup_func(device, temp, dst);
    }
    else
    {
      tier1::binary_infsup_func(device, dst, temp);
      tier1::binary_supinf_func(device, temp, dst);
    }
  }
}


auto
morphological_chan_vese_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             int                     num_iter,
                             int                     smoothing,
                             float                   lambda1,
                             float                   lambda2) -> Array::Pointer
{
  // WARINING: dst MUST be binary
  if (dst == nullptr)
  {
    // dst is the initialisation contour
    // if not provided, use a checkerboard pattern as initialisation
    tier0::create_like(src, dst, dType::BINARY);
    create_checkerboard_init(src, dst, 5);
  }
  // enforce contour (dst) to be binary
  tier1::greater_constant_func(device, dst, dst, 0);

  float c0 = 0;
  float c1 = 0;
  auto  outside_contour = Array::create(dst);
  auto  gradient_magnitude =
    Array::create(dst->width(), dst->height(), dst->depth(), dst->dimension(), dType::FLOAT, mType::BUFFER, device);

  int ite = 0;
  while (ite < num_iter)
  {
    // compute of inside contour score
    compute_contour_score(device, src, dst, c1);
    // compute of outside contour score (on inverted dst)
    tier1::binary_not_func(device, dst, outside_contour);
    compute_contour_score(device, src, outside_contour, c0);

    // compute gradient magnitude into temp_3
    compute_gradient_magnitude(device, dst, gradient_magnitude);

    // compute contour evolution according to gradient and score on contour
    compute_contour_evolution(device, src, gradient_magnitude, c0, c1, lambda1, lambda2);

    // apply contour update on contour image
    apply_contour_evolution(device, gradient_magnitude, dst);

    // smooth contour
    smooth_contour(device, dst, smoothing);

    ite++;
  }
  return dst;
}

} // namespace cle::tier3
