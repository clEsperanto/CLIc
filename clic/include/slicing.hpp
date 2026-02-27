#pragma once

#include "array.hpp"

#include <algorithm>
#include <array>
#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <vector>

namespace cle
{

/**
 * @brief Represents a single-axis slice specification, analogous to Python's slice(start, stop, step).
 *
 * Supports:
 *   - Full axis:        Slice()                   equivalent to [:]
 *   - Single index:     Slice(5)                  equivalent to [5]   (reduces dimension)
 *   - Range:            Slice(1, 10)              equivalent to [1:10]
 *   - Range with step:  Slice(1, 10, 2)           equivalent to [1:10:2]
 *   - From start:       Slice(std::nullopt, 5)    equivalent to [:5]
 *   - To end:           Slice(3, std::nullopt)    equivalent to [3:]
 *   - Negative indices: Slice(-3, std::nullopt)   equivalent to [-3:]
 */
struct Slice
{
  std::optional<int> start;
  std::optional<int> stop;
  int                step = 1;
  bool               is_index = false;

  /** Full axis [:] */
  Slice()
    : start(std::nullopt)
    , stop(std::nullopt)
    , step(1)
    , is_index(false)
  {}

  /** Single index [i] — collapses this axis */
  explicit Slice(int index)
    : start(index)
    , stop(std::nullopt)
    , step(1)
    , is_index(true)
  {}

  /** Range [start:stop] */
  Slice(std::optional<int> start_, std::optional<int> stop_)
    : start(start_)
    , stop(stop_)
    , step(1)
    , is_index(false)
  {}

  /** Range with step [start:stop:step] */
  Slice(std::optional<int> start_, std::optional<int> stop_, int step_)
    : start(start_)
    , stop(stop_)
    , step(step_)
    , is_index(false)
  {}

  /**
   * @brief Resolve the slice against a concrete axis length.
   *
   * Follows CPython's slice.indices(length) semantics exactly.
   * Returns absolute start, stop, step values. For negative steps,
   * stop can be -1 (meaning "before element 0").
   *
   * @param axis_len Length of the axis this slice is applied to.
   * @return std::tuple<int, int, int>  (resolved_start, resolved_stop, resolved_step)
   */
  [[nodiscard]] auto
  resolve(size_t axis_len) const -> std::tuple<int, int, int>
  {
    const int len = static_cast<int>(axis_len);

    if (step == 0)
    {
      throw std::invalid_argument("Slice step cannot be zero.");
    }

    if (is_index)
    {
      int idx = start.value();
      if (idx < 0)
        idx += len;
      if (idx < 0 || idx >= len)
      {
        throw std::out_of_range("Slice index " + std::to_string(start.value()) +
                                " is out of range for axis of length " + std::to_string(len));
      }
      return { idx, idx + 1, 1 };
    }

    // Follow CPython's slice.indices(length) algorithm
    int r_step = step;

    // Determine lower and upper bounds for clamping
    int lower, upper;
    if (r_step > 0)
    {
      lower = 0;
      upper = len;
    }
    else
    {
      lower = -1;
      upper = len - 1;
    }

    // Resolve start
    int r_start;
    if (start.has_value())
    {
      r_start = start.value();
      if (r_start < 0)
        r_start += len;
      r_start = std::clamp(r_start, lower, upper);
    }
    else
    {
      r_start = (r_step > 0) ? lower : upper;
    }

    // Resolve stop
    int r_stop;
    if (stop.has_value())
    {
      r_stop = stop.value();
      if (r_stop < 0)
        r_stop += len;
      r_stop = std::clamp(r_stop, lower, upper);
    }
    else
    {
      r_stop = (r_step > 0) ? upper : lower;
    }

    return { r_start, r_stop, r_step };
  }

  /**
   * @brief Number of elements this slice selects along an axis of the given length.
   *
   * Follows the same formula as CPython: max(0, (stop - start + step - sign(step)) / step)
   */
  [[nodiscard]] auto
  output_length(size_t axis_len) const -> size_t
  {
    auto [r_start, r_stop, r_step] = resolve(axis_len);

    if (r_step > 0)
    {
      if (r_stop <= r_start)
        return 0;
      return static_cast<size_t>((r_stop - r_start + r_step - 1) / r_step);
    }
    else
    {
      if (r_start <= r_stop)
        return 0;
      // r_step is negative; (r_start - r_stop) is positive, (-r_step) is positive
      return static_cast<size_t>((r_start - r_stop - r_step - 1) / (-r_step));
    }
  }
};


// ─── Convenience free-function constructors ─────────────────────────────────

/** Full axis [:] */
inline auto
S_() -> Slice
{
  return Slice();
}

/** Single index [i] */
inline auto
S_(int index) -> Slice
{
  return Slice(index);
}

/** Range [start:stop] */
inline auto
S_(std::optional<int> start, std::optional<int> stop) -> Slice
{
  return Slice(start, stop);
}

/** Range with step [start:stop:step] */
inline auto
S_(std::optional<int> start, std::optional<int> stop, int step) -> Slice
{
  return Slice(start, stop, step);
}


// ─── Resolved slice info (internal) ────────────────────────────────────────

/** @brief Internal helper: fully resolved slice for one axis. All values are plain ints. */
struct ResolvedSlice
{
  int    start;    // absolute start index (can be -1 only as neg-step sentinel)
  int    stop;     // absolute stop index  (exclusive; can be -1 for neg-step)
  int    step;
  size_t length;   // number of output elements
  bool   is_index; // if true, this axis is collapsed in the output
};


// ─── The main slicing / pasting functions ───────────────────────────────────

auto
slice(const Array::Pointer & src, const std::vector<Slice> & slices) -> Array::Pointer;

auto
slice(const Array::Pointer & src,
      const Slice & x_slice = Slice(),
      const Slice & y_slice = Slice(),
      const Slice & z_slice = Slice()) -> Array::Pointer;

auto
paste(const Array::Pointer & src, const Array::Pointer & dst, const std::vector<Slice> & slices) -> void;

auto
paste(const Array::Pointer & src, const Array::Pointer & dst,
      const Slice & x_slice = Slice(),
      const Slice & y_slice = Slice(),
      const Slice & z_slice = Slice()) -> void;

} // namespace cle