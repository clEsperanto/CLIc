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
 *   - Full axis:        Slice()          equivalent to [:]
 *   - Single index:     Slice(5)         equivalent to [5]   (reduces dimension)
 *   - Range:            Slice(1, 10)     equivalent to [1:10]
 *   - Range with step:  Slice(1, 10, 2)  equivalent to [1:10:2]
 *   - From start:       Slice(std::nullopt, 5)  equivalent to [:5]
 *   - To end:           Slice(3, std::nullopt)  equivalent to [3:]
 *   - Negative indices: Slice(-3, std::nullopt) equivalent to [-3:]
 */
struct Slice
{
  std::optional<int> start;
  std::optional<int> stop;
  int                step = 1;
  bool               is_index = false; // true when a single integer index was given (reduces dim)

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
   * After resolution the returned tuple contains absolute (non-negative)
   * start, stop and step values that are clamped to [0, axis_len].
   *
   * @param axis_len Length of the axis this slice is applied to.
   * @return std::tuple<size_t, size_t, int>  (resolved_start, resolved_stop, resolved_step)
   */
  [[nodiscard]] auto
  resolve(size_t axis_len) const -> std::tuple<size_t, size_t, int>
  {
    const int len = static_cast<int>(axis_len);

    if (step == 0)
    {
      throw std::invalid_argument("Slice step cannot be zero.");
    }

    // ── helper: clamp a Python-style (possibly negative) index ──
    auto clamp = [&](int idx, int low, int high) -> int {
      if (idx < 0)
        idx += len;
      return std::clamp(idx, low, high);
    };

    int r_start, r_stop;

    if (is_index)
    {
      // Single index — resolve negative, bounds-check, return width-1 range
      int idx = start.value();
      if (idx < 0)
        idx += len;
      if (idx < 0 || idx >= len)
      {
        throw std::out_of_range("Slice index " + std::to_string(start.value()) + " is out of range for axis of length " +
                                std::to_string(len));
      }
      return { static_cast<size_t>(idx), static_cast<size_t>(idx + 1), 1 };
    }

    if (step > 0)
    {
      r_start = start.has_value() ? clamp(start.value(), 0, len) : 0;
      r_stop = stop.has_value() ? clamp(stop.value(), 0, len) : len;
      if (r_stop < r_start)
        r_stop = r_start; // empty range
    }
    else
    {
      // Negative step (e.g. [::-1])
      r_start = start.has_value() ? clamp(start.value(), -1, len - 1) : (len - 1);
      r_stop = stop.has_value() ? clamp(stop.value(), -1, len - 1) : -1;
      if (r_stop > r_start)
        r_stop = r_start; // empty range
    }

    return { static_cast<size_t>(std::max(r_start, 0)),
             static_cast<size_t>(std::max(r_stop, -1)), // -1 is valid sentinel for neg step
             step };
  }

  /**
   * @brief Number of elements this slice selects along an axis of the given length.
   */
  [[nodiscard]] auto
  output_length(size_t axis_len) const -> size_t
  {
    auto [r_start, r_stop, r_step] = resolve(axis_len);
    if (r_step > 0)
    {
      if (r_stop <= r_start)
        return 0;
      return (r_stop - r_start + static_cast<size_t>(r_step) - 1) / static_cast<size_t>(r_step);
    }
    else
    {
      // For negative steps r_start > r_stop (r_stop may be SIZE_MAX wrapping from -1)
      int diff = static_cast<int>(r_start) - static_cast<int>(r_stop);
      if (diff <= 0)
        return 0;
      return (static_cast<size_t>(diff) + static_cast<size_t>(-r_step) - 1) / static_cast<size_t>(-r_step);
    }
  }
};


// ─── Convenience free-function constructors (short names) ───────────────────

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

// ─── Convenience aliases using an even shorter "_" ──────────────────────────
// Usage:  _()  _(5)  _(1,10)  _(1,10,2)  _({},5)  _(3,{})

/**
 * @brief All-in-one alias.  Use std::nullopt or {} for "unspecified".
 *
 *   _()            →  [:]
 *   _(3)           →  [3]       (collapses axis)
 *   _(1, 5)        →  [1:5]
 *   _(1, 5, 2)     →  [1:5:2]
 *   _({}, 5)       →  [:5]
 *   _(3, {})       →  [3:]
 *   _(-3, {})      →  [-3:]
 */


// ─── Resolved slice info (internal) ────────────────────────────────────────

/** @brief Internal helper: fully resolved slice for one axis */
struct ResolvedSlice
{
  size_t start;
  size_t stop;
  int    step;
  size_t length;   // number of output elements
  bool   is_index; // if true, this axis is collapsed in the output
};


// ─── The main slicing / pasting function ─────────────────────────────────────────────

/**
 * @brief Extract a sub-array from `src` described by up to 3 Slice specs (x, y, z order).
 *
 * Axes that are not mentioned default to [:] (full axis).
 * An axis given as a single index (Slice(i)) is **collapsed**: the output
 * has one fewer dimension (minimum 1).
 *
 * ## Examples (assuming `img` is a 3-D Array with shape 256×256×100 in x,y,z)
 *
 * ```cpp
 * // Get a single z-slice → 2-D (256×256)
 * auto plane = slice(img, S_(), S_(), S_(50));
 *
 * // Crop a region → 3-D (100×100×100)
 * auto crop = slice(img, S_(0, 100), S_(0, 100), S_(0, 100));
 *
 * // Every other column → 3-D (128×256×100)
 * auto cols = slice(img, S_(0, {}, 2), S_(), S_());
 *
 * // Last 10 z-planes
 * auto tail = slice(img, S_(), S_(), S_(-10, {}));
 * ```
 *
 * @param src     Source array
 * @param slices  Vector of Slice descriptors, one per axis (x, y, z). Missing trailing axes → [:]
 * @return Array::Pointer  Newly allocated array holding the sliced data.
 */
auto
slice(const Array::Pointer & src, const std::vector<Slice> & slices) -> Array::Pointer;

/** Overload with up to 3 individual Slice arguments (most common usage) */
auto
slice(const Array::Pointer & src, const Slice & x_slice = Slice(), const Slice & y_slice = Slice(), const Slice & z_slice = Slice())
  -> Array::Pointer;

/**
 * @brief Write src INTO a sub-region of dst described by up to 3 Slice specs (x, y, z order).
 *
 * This is the inverse of slice(): where slice() reads a region out,
 * paste() writes data in.
 *
 * The source array dimensions must exactly match the region described
 * by the slices (with collapsed axes having size 1).
 *
 * @param src     Data to write (small array)
 * @param dst     Target array (modified in-place)
 * @param slices  Where in dst to write (x, y, z order)
 */
auto
paste(const Array::Pointer & src, const Array::Pointer & dst, const std::vector<Slice> & slices) -> void;

auto
paste(const Array::Pointer & src, const Array::Pointer & dst,
      const Slice & x_slice = Slice(), const Slice & y_slice = Slice(), const Slice & z_slice = Slice()) -> void;

} // namespace cle
