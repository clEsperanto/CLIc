#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <tuple>

namespace cle::tier3
{

auto
coordinate_generator(int width, int height, int depth,
                     float delta_x, float delta_y, float delta_z,
                     float sigma_x, float sigma_y, float sigma_z) -> std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> 
{
    std::vector<float> all_x_coords;
    std::vector<float> all_y_coords;
    std::vector<float> all_z_coords;
    
    // Create random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist_x(0.0f, sigma_x);
    std::normal_distribution<float> dist_y(0.0f, sigma_y);
    std::normal_distribution<float> dist_z(0.0f, sigma_z);
    
    int k = 0;
    for (float z = 0; z < depth; z += delta_z) {
        int i = 0;
        for (float y = 0; y < height; y += delta_y) {
            // Alternate offset pattern for hexagonal/honeycomb structure
            float offset_x = 0.0f;
            if (i % 2 != 0) {
                offset_x = delta_x / 2.0f;
            }
            if (k % 2 != 0) {
                offset_x += delta_x / 4.0f;
            }
            
            // Generate coordinates for this row
            std::vector<float> x_coords;
            std::vector<float> y_coords;
            std::vector<float> z_coords;
            
            for (float x = offset_x; x < width; x += delta_x) {
                x_coords.push_back(x);
            }
            
            size_t num_coords = x_coords.size();
            y_coords.resize(num_coords, y);
            z_coords.resize(num_coords, z);
            
            // Add random noise to coordinates
            for (size_t j = 0; j < num_coords; ++j) {
                x_coords[j] += dist_x(gen);
                y_coords[j] += dist_y(gen);
                z_coords[j] += dist_z(gen);
            }
            
            // Append to all coordinates
            all_x_coords.insert(all_x_coords.end(), x_coords.begin(), x_coords.end());
            all_y_coords.insert(all_y_coords.end(), y_coords.begin(), y_coords.end());
            all_z_coords.insert(all_z_coords.end(), z_coords.begin(), z_coords.end());
            
            ++i;
        }
        ++k;
    }
    
    return {all_x_coords, all_y_coords, all_z_coords};
}


auto
artificial_tissue_func(const Device::Pointer & device,
                      int width,
                      int height,
                      int depth,
                      float                    delta_x,
                      float                    delta_y,
                      float                    delta_z,
                      float                    sigma_x,
                      float                    sigma_y,
                      float                    sigma_z) -> Array::Pointer
{
    auto dim = (depth > 1) ? 3 : (height > 1) ? 2 : 1;
    auto dst = Array::create(width, height, depth, dim, dType::LABEL, mType::BUFFER, device);

    auto [all_x_coords, all_y_coords, all_z_coords] = coordinate_generator(
        width, height, depth, delta_x, delta_y, delta_z, sigma_x, sigma_y, sigma_z
    );

    auto nb_points = all_x_coords.size();
    for (size_t i = 0; i < nb_points; ++i) {
        all_x_coords[i] = std::clamp(all_x_coords[i], 0.0f, static_cast<float>(width - 1));
        all_y_coords[i] = std::clamp(all_y_coords[i], 0.0f, static_cast<float>(height - 1));
        all_z_coords[i] = std::clamp(all_z_coords[i], 0.0f, static_cast<float>(depth - 1));
    }

    auto point_list = Array::create(nb_points, 3, 1, 2, dType::FLOAT, mType::BUFFER, device);
    point_list->writeFrom(all_x_coords.data(), {nb_points, 1, 1}, {0, 0, 0});
    point_list->writeFrom(all_y_coords.data(), {nb_points, 1, 1}, {0, 1, 0});
    point_list->writeFrom(all_z_coords.data(), {nb_points, 1, 1}, {0, 2, 0});

    auto centroids = Array::create(dst);
    tier2::pointlist_to_labelled_spots_func(device, point_list, centroids);
    tier2::extend_labeling_via_voronoi_func(device, centroids, dst);
    
    return dst;
}


} // namespace cle::tier3