#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
create_checkerboard_init(const Array::Pointer & src, Array::Pointer dst, int squareSize) -> void
{
  if (dst != nullptr)
  {
    return;
  }
  tier0::create_like(src, dst, dType::BINARY);

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
  dst->write(checkerboard.data());
}

auto
compute_contour_score(const Device::Pointer & device,
                      const Array::Pointer &  image,
                      const Array::Pointer &  contour,
                      float &                 c) -> float
{
  auto masked = tier1::mask_func(device, image, contour, nullptr);
  auto sum_image_value = tier2::sum_of_all_pixels_func(device, masked);
  auto sum_contour_value = tier2::sum_of_all_pixels_func(device, contour) + 1e-8;
  return -(sum_image_value / sum_contour_value);
}

auto
compute_gradient_magnitude(const Device::Pointer & device, const Array::Pointer & contour, Array::Pointer dst) -> void
{
  auto temp_1 = Array::create(dst);
  auto temp_2 = Array::create(dst);
  dst->fill(0);
  for (int d = 0; d < contour->dimension(); d++)
  {
    switch (d)
    {
      case 0:
        tier1::gradient_x_func(device, contour, temp_1);
        break;
      case 1:
        tier1::gradient_y_func(device, contour, temp_1);
        break;
      case 2:
        tier1::gradient_z_func(device, contour, temp_1);
        break;
    }
    tier1::absolute_func(device, temp_1, temp_2);
    tier1::add_images_weighted_func(device, temp_2, dst, dst, 1, 1);
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
  auto temp_1 = Array::create(evolution);
  auto temp_2 = Array::create(evolution);
  auto temp_3 = Array::create(evolution);
  tier1::add_image_and_scalar_func(device, image, temp_1, c1);
  tier1::add_image_and_scalar_func(device, image, temp_2, c0);
  tier1::power_func(device, temp_1, temp_1, 2);
  tier1::power_func(device, temp_2, temp_2, 2);
  tier1::add_images_weighted_func(device, temp_1, temp_2, temp_3, lambda1, -lambda2);
  tier1::multiply_images_func(device, temp_3, evolution, evolution);
}

auto
apply_contour_evolution(const Device::Pointer & device,
                        const Array::Pointer &  evolution,
                        Array::Pointer          contour) -> void
{
  auto temp_1 = Array::create(contour);
  auto temp_2 = Array::create(contour);
  auto temp_3 = Array::create(contour);

  tier1::greater_constant_func(device, evolution, temp_1, 0);
  tier1::smaller_constant_func(device, evolution, temp_2, 0);

  tier1::binary_or_func(device, temp_1, temp_2, temp_3);
  tier1::binary_not_func(device, temp_3, temp_1);

  tier1::mask_func(device, contour, temp_1, temp_2);
  tier1::add_images_weighted_func(device, temp_2, temp_3, contour, 1, 1);
}

auto
smooth_contour(const Device::Pointer & device, Array::Pointer dst, int iteration) -> void
{
  if (iteration == 0)
  {
    return;
  }
  auto temp_1 = Array::create(dst);
  for (int i = 0; i < iteration; i++)
  {
    if (i % 2 == 0)
    {
      tier1::binary_infsup_func(device, dst, temp_1);
      tier1::binary_supinf_func(device, temp_1, dst);
    }
    else
    {
      tier1::binary_supinf_func(device, dst, temp_1);
      tier1::binary_infsup_func(device, temp_1, dst);
    }
  }
}


auto
morphological_chan_vese_func(const Device::Pointer & device,
                             const Array::Pointer &  src,
                             Array::Pointer          dst,
                             int                     number_iteration,
                             int                     smoothing_iteration,
                             float                   lambda1,
                             float                   lambda2) -> Array::Pointer
{
  // check if dst is nullptr otherwise create a new array and
  // initialize it with a checkerboard pattern
  create_checkerboard_init(src, dst, 5);

  // needed?
  tier1::greater_constant_func(device, dst, dst, 0);

  float c0 = 0;
  float c1 = 0;
  auto  outside_contour = Array::create(dst);
  auto  gradient = Array::create(dst);

  int ite = 0;
  while (ite < number_iteration)
  {
    // compute of inside (dst) and outside contour (temp_1)
    tier1::binary_not_func(device, dst, outside_contour);
    // compute of outside contour score
    compute_contour_score(device, src, outside_contour, c0);
    // compute of inside contour score
    compute_contour_score(device, src, dst, c1);
    // compute gradient magnitude into temp_3
    compute_gradient_magnitude(device, dst, gradient);
    // compute contour evolution according to gradient and score on contour
    compute_contour_evolution(device, src, gradient, c0, c1, lambda1, lambda2);
    // apply contour update on contour image
    apply_contour_evolution(device, gradient, dst);
    // smooth contour
    smooth_contour(device, dst, smoothing_iteration);

    ite++;
  }

  return dst;
}

} // namespace cle::tier3
