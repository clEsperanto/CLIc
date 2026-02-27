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
    const std::string src_type = toString(src->dtype());
    const std::string dst_type = toString(dst->dtype());

    const std::string convert_suffix = (dst->dtype() == dType::FLOAT) ? "" : "_sat";
    const std::string kernel_name = "copy_cast_" + src_type + "_to_" + dst_type;

    const std::string kernel_source =
      "__kernel void " + kernel_name + "("
      "  __global const " + src_type + "* src,"
      "  __global "       + dst_type + "* dst,"
      "  const unsigned long size"
      ") {\n"
      "  const size_t idx = get_global_id(0);\n"
      "  if (idx < size) { dst[idx] = convert_" + dst_type + convert_suffix + "(src[idx]); }\n"
      "}\n";

    const size_t     total_size = src->size();
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
