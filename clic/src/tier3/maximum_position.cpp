#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp" 

#include "utils.hpp"

namespace cle::tier3 
{

auto
maximum_position_func(const Device::Pointer & device, const Array::Pointer & src) -> std::array<size_t, 3>
{
  size_t                z_coord = 0;
  size_t                y_coord = 0;
  size_t                x_coord = 0;
  std::array<size_t, 3> coord = { 0, 0, 0 };

  Array::Pointer pos_x;
  Array::Pointer pos_y;
  Array::Pointer pos_z;
  Array::Pointer temp = src;

  if (src->depth() > 1)
  {
    pos_z = tier1::z_position_of_maximum_z_projection_func(device, temp, nullptr);
    temp = tier1::maximum_z_projection_func(device, temp, nullptr);
  }
  if (src->height() > 1)
  {
    pos_y = tier1::y_position_of_maximum_y_projection_func(device, temp, nullptr);
    temp = tier1::maximum_y_projection_func(device, temp, nullptr);
  }
  pos_x = tier1::x_position_of_maximum_x_projection_func(device, temp, nullptr);
  temp = tier1::maximum_x_projection_func(device, temp, nullptr);


  if (pos_x != nullptr)
  {
    pos_x->read(&x_coord, { 1, 1, 1 }, { 0, 0, 0 });
    coord[0] = x_coord;
  }
  if (pos_y != nullptr)
  {
    pos_y->read(&y_coord, { 1, 1, 1 }, { x_coord, 0, 0 });
    coord[1] = y_coord;
  }
  if (pos_z != nullptr)
  {
    pos_z->read(&z_coord, { 1, 1, 1 }, { x_coord, y_coord, 0 });
    coord[2] = z_coord;
  }
  return coord;
}  

}

