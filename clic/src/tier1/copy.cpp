#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

// #include "cle_copy_func.h"
// #include "cle_copy_horizontal_slice_from.h"
// #include "cle_copy_horizontal_slice_to.h"
// #include "cle_copy_slice_from.h"
// #include "cle_copy_slice_to.h"
// #include "cle_copy_vertical_slice_from.h"
// #include "cle_copy_vertical_slice_to.h"

namespace cle::tier1
{

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  if (src->dtype() == dst->dtype())
  {
    src->copyTo(dst);
  }
  else
  {
    // Generate a minimal 1D native kernel for type casting.
    // The kernel source depends only on (src_type, dst_type), maximising cache reuse regardless of array dimensions or sizes.
    // - CLIJ copy kernel is too specific for such a general purpose function, and would not be reused enough
    const bool        is_opencl = (device->getType() == Device::Type::OPENCL);
    const std::string src_type = toString(src->dtype());
    const std::string dst_type = toString(dst->dtype());
    const std::string addr = is_opencl ? "__global " : "";

    const std::string kernel_source = "__kernel void copy_cast( __global const " + src_type + "* src,  __global " + dst_type +
                                      "* dst, const int size) {\n" + "  const int idx = get_global_id(0);\n" +
                                      "  if (idx < size) { dst[idx] = (" + dst_type + ")src[idx]; }\n}\n";
    const std::string kernel_name = "copy_cast";

    const int        total_size = static_cast<int>(src->size());
    const size_t     max_local = device->getMaximumWorkGroupSize();
    const size_t     global_padded = ((total_size + max_local - 1) / max_local) * max_local;
    const RangeArray global_range = { global_padded, 1, 1 };
    const RangeArray local_range = { max_local, 1, 1 };

    const KernelInfo    kernel = { kernel_name, kernel_source };
    const ParameterList params = { { "src", src }, { "dst", dst }, { "size", total_size } };
    native_execute(device, kernel, params, global_range, local_range);
  }
  return dst;
}

} // namespace cle::tier1
