#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestSlicing : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

// ─────────────────────────────────────────────────────────────────────────────
// Basic slicing tests - full axis / contiguous paths
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_full_axis)
{
  // Create a 3D array and slice the full axis (should return a copy)
  std::array<float, 2 * 3 * 4> data;
  for (int i = 0; i < 2 * 3 * 4; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(2, 3, 4, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice with full axes [:]
  auto dst = cle::slice(src, { cle::Slice(), cle::Slice(), cle::Slice() });

  EXPECT_EQ(dst->width(), 2);
  EXPECT_EQ(dst->height(), 3);
  EXPECT_EQ(dst->depth(), 4);
  EXPECT_EQ(dst->dtype(), cle::dType::FLOAT);

  // Read back and verify all data
  std::array<float, 2 * 3 * 4> result;
  dst->readTo(result.data());
  for (int i = 0; i < 2 * 3 * 4; i++)
  {
    EXPECT_EQ(result[i], data[i]);
  }
}

TEST_P(TestSlicing, slice_range_contiguous)
{
  // Create a 3D array and slice a contiguous range
  std::array<float, 5 * 5 * 5> data;
  for (int i = 0; i < 5 * 5 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(5, 5, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1:4, 1:4, 1:4] — contiguous, step=1
  auto dst = cle::slice(src, { cle::Slice(1, 4), cle::Slice(1, 4), cle::Slice(1, 4) });

  EXPECT_EQ(dst->width(), 3);
  EXPECT_EQ(dst->height(), 3);
  EXPECT_EQ(dst->depth(), 3);

  std::array<float, 3 * 3 * 3> result;
  dst->readTo(result.data());

  // Verify specific elements
  int idx = 0;
  for (int z = 1; z < 4; z++)
  {
    for (int y = 1; y < 4; y++)
    {
      for (int x = 1; x < 4; x++)
      {
        int src_idx = z * 5 * 5 + y * 5 + x;
        EXPECT_EQ(result[idx], data[src_idx]);
        idx++;
      }
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Strided slicing tests - non-contiguous with steps
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_with_step)
{
  // Create a 1D array and slice every other element
  std::array<float, 10> data;
  for (int i = 0; i < 10; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [0:10:2] — every other element
  auto dst = cle::slice(src, { cle::Slice(0, 10, 2), cle::Slice(), cle::Slice() });

  EXPECT_EQ(dst->width(), 5);
  EXPECT_EQ(dst->height(), 1);
  EXPECT_EQ(dst->depth(), 1);

  std::array<float, 5> result;
  dst->readTo(result.data());

  for (int i = 0; i < 5; i++)
  {
    EXPECT_EQ(result[i], static_cast<float>(i * 2));
  }
}

TEST_P(TestSlicing, slice_with_step_2d)
{
  // Create a 2D array and slice with steps on both axes
  std::array<float, 6 * 6> data;
  for (int i = 0; i < 6 * 6; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(6, 6, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [0:6:2, 0:6:2] — every other element in both dimensions
  auto dst = cle::slice(src, { cle::Slice(0, 6, 2), cle::Slice(0, 6, 2), cle::Slice() });

  EXPECT_EQ(dst->width(), 3);
  EXPECT_EQ(dst->height(), 3);
  EXPECT_EQ(dst->depth(), 1);

  std::array<float, 3 * 3> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 0; y < 6; y += 2)
  {
    for (int x = 0; x < 6; x += 2)
    {
      int src_idx = y * 6 + x;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Index-based slicing (dimension reduction)
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_single_index_x)
{
  // Create a 3D array and select a single X index — should reduce to 2D
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1, :, :] — select X=1, keep Y and Z
  auto dst = cle::slice(src, { cle::Slice(1), cle::Slice(), cle::Slice() });

  EXPECT_EQ(dst->width(), 1);  // X dimension collapsed
  EXPECT_EQ(dst->height(), 4); // Y dimension full
  EXPECT_EQ(dst->depth(), 5);  // Z dimension full

  std::array<float, 1 * 4 * 5> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int z = 0; z < 5; z++)
  {
    for (int y = 0; y < 4; y++)
    {
      int src_idx = z * 3 * 4 + y * 3 + 1;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

TEST_P(TestSlicing, slice_single_index_z)
{
  // Create a 3D array and select a single Z index — should reduce to 2D
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [:, :, 2] — select Z=2, keep X and Y
  auto dst = cle::slice(src, { cle::Slice(), cle::Slice(), cle::Slice(2) });

  EXPECT_EQ(dst->width(), 3);  // X dimension full
  EXPECT_EQ(dst->height(), 4); // Y dimension full
  EXPECT_EQ(dst->depth(), 1);  // Z dimension collapsed
  EXPECT_EQ(dst->dim(), 2);    // Should be 2D

  std::array<float, 3 * 4 * 1> result;
  dst->readTo(result.data());

  int idx = 0;
  int z = 2;
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 3; x++)
    {
      int src_idx = z * 3 * 4 + y * 3 + x;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

TEST_P(TestSlicing, slice_multiple_indices)
{
  // Create a 3D array and select single indices on X and Z — should reduce to 1D
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1, :, 2] — select X=1 and Z=2, keep only Y
  auto dst = cle::slice(src, { cle::Slice(1), cle::Slice(), cle::Slice(2) });

  EXPECT_EQ(dst->width(), 1);  // X dimension collapsed
  EXPECT_EQ(dst->height(), 4); // Y dimension full
  EXPECT_EQ(dst->depth(), 1);  // Z dimension collapsed

  std::array<float, 1 * 4 * 1> result;
  dst->readTo(result.data());

  int idx = 0;
  int z = 2, x = 1;
  for (int y = 0; y < 4; y++)
  {
    int src_idx = z * 3 * 4 + y * 3 + x;
    EXPECT_EQ(result[idx], data[src_idx]);
    idx++;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Partial slicing (fewer than 3 axes specified)
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_2axes)
{
  // Create a 3D array but only specify 2 slice specs — third defaults to [:]
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1:3, 1:3] — implicit [:] for Z
  auto dst = cle::slice(src, { cle::Slice(1, 3), cle::Slice(1, 3) });

  EXPECT_EQ(dst->width(), 2);
  EXPECT_EQ(dst->height(), 2);
  EXPECT_EQ(dst->depth(), 5); // Full Z range
}

TEST_P(TestSlicing, slice_1axis)
{
  // Create a 3D array but only specify 1 slice spec
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1:3] — implicit [:] for Y and Z
  auto dst = cle::slice(src, { cle::Slice(1, 3) });

  EXPECT_EQ(dst->width(), 2);
  EXPECT_EQ(dst->height(), 4); // Full Y range
  EXPECT_EQ(dst->depth(), 5);  // Full Z range
}

// ─────────────────────────────────────────────────────────────────────────────
// Negative indices and ranges
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_negative_index)
{
  // Create a 1D array and use negative index
  std::array<float, 10> data;
  for (int i = 0; i < 10; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [-3:] — last 3 elements
  auto dst = cle::slice(src, { cle::Slice(-3, std::nullopt) });

  EXPECT_EQ(dst->width(), 3);

  std::array<float, 3> result;
  dst->readTo(result.data());

  for (int i = 0; i < 3; i++)
  {
    EXPECT_EQ(result[i], static_cast<float>(7 + i));
  }
}

TEST_P(TestSlicing, slice_negative_stop)
{
  // Create a 1D array and use negative stop
  std::array<float, 10> data;
  for (int i = 0; i < 10; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [:-3] — all but last 3 elements (equivalent to [:7])
  auto dst = cle::slice(src, { cle::Slice(std::nullopt, -3) });

  EXPECT_EQ(dst->width(), 7);

  std::array<float, 7> result;
  dst->readTo(result.data());

  for (int i = 0; i < 7; i++)
  {
    EXPECT_EQ(result[i], static_cast<float>(i));
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Different data types
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_uint8)
{
  std::array<uint8_t, 4 * 4> data;
  for (int i = 0; i < 4 * 4; i++)
  {
    data[i] = static_cast<uint8_t>(i);
  }

  auto src = cle::Array::create(4, 4, 1, 2, cle::dType::UINT8, cle::mType::BUFFER, data.data(), device);
  auto dst = cle::slice(src, { cle::Slice(1, 3), cle::Slice(1, 3) });

  EXPECT_EQ(dst->dtype(), cle::dType::UINT8);
  EXPECT_EQ(dst->width(), 2);
  EXPECT_EQ(dst->height(), 2);

  std::array<uint8_t, 2 * 2> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 1; y < 3; y++)
  {
    for (int x = 1; x < 3; x++)
    {
      int src_idx = y * 4 + x;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

TEST_P(TestSlicing, slice_int32)
{
  std::array<int32_t, 5 * 5> data;
  for (int i = 0; i < 5 * 5; i++)
  {
    data[i] = static_cast<int32_t>(i - 12);
  }

  auto src = cle::Array::create(5, 5, 1, 2, cle::dType::INT32, cle::mType::BUFFER, data.data(), device);
  auto dst = cle::slice(src, { cle::Slice(2, 4), cle::Slice(1, 4), cle::Slice() });

  EXPECT_EQ(dst->dtype(), cle::dType::INT32);
  EXPECT_EQ(dst->width(), 2);
  EXPECT_EQ(dst->height(), 3);

  std::array<int32_t, 2 * 3> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 1; y < 4; y++)
  {
    for (int x = 2; x < 4; x++)
    {
      int src_idx = y * 5 + x;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Edge cases
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_single_element)
{
  // Create a 3D array and select a single element using indices
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1, 2, 3] — single element
  auto dst = cle::slice(src, { cle::Slice(1), cle::Slice(2), cle::Slice(3) });

  EXPECT_EQ(dst->width(), 1);
  EXPECT_EQ(dst->height(), 1);
  EXPECT_EQ(dst->depth(), 1);

  std::array<float, 1> result;
  dst->readTo(result.data());

  int src_idx = 3 * 3 * 4 + 2 * 3 + 1;
  EXPECT_EQ(result[0], data[src_idx]);
}

TEST_P(TestSlicing, slice_partial_coverage)
{
  // Create a 2D array and slice a small portion
  std::array<float, 10 * 10> data;
  for (int i = 0; i < 10 * 10; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(10, 10, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);
  auto dst = cle::slice(src, { cle::Slice(2, 5), cle::Slice(3, 7) });

  EXPECT_EQ(dst->width(), 3);
  EXPECT_EQ(dst->height(), 4);

  std::array<float, 3 * 4> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 3; y < 7; y++)
  {
    for (int x = 2; x < 5; x++)
    {
      int src_idx = y * 10 + x;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Convenience overloads
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_overload_xyz)
{
  // Test the overload with three individual Slice arguments
  std::array<float, 3 * 4 * 5> data;
  for (int i = 0; i < 3 * 4 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(3, 4, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Use the overload: slice(src, x_slice, y_slice, z_slice)
  auto dst = cle::slice(src, cle::Slice(1, 3), cle::Slice(1, 3), cle::Slice(1, 4));

  EXPECT_EQ(dst->width(), 2);
  EXPECT_EQ(dst->height(), 2);
  EXPECT_EQ(dst->depth(), 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// Negative step (reverse) slicing
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_negative_step_reverse_1d)
{
  // Create a 1D array and reverse it with step=-1
  std::array<float, 6> data = { 0, 1, 2, 3, 4, 5 };

  auto src = cle::Array::create(6, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [::-1] — full reverse
  auto dst = cle::slice(src, { cle::Slice(std::nullopt, std::nullopt, -1) });

  EXPECT_EQ(dst->width(), 6);

  std::array<float, 6> result;
  dst->readTo(result.data());

  for (int i = 0; i < 6; i++)
  {
    EXPECT_EQ(result[i], static_cast<float>(5 - i));
  }
}

TEST_P(TestSlicing, slice_negative_step_partial)
{
  // Reverse a sub-range: [4:1:-1] → elements at indices 4, 3, 2
  std::array<float, 8> data = { 0, 1, 2, 3, 4, 5, 6, 7 };

  auto src = cle::Array::create(8, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  auto dst = cle::slice(src, { cle::Slice(4, 1, -1) });

  EXPECT_EQ(dst->width(), 3);

  std::array<float, 3> result;
  dst->readTo(result.data());

  EXPECT_EQ(result[0], 4.0f);
  EXPECT_EQ(result[1], 3.0f);
  EXPECT_EQ(result[2], 2.0f);
}

TEST_P(TestSlicing, slice_negative_step_every_other)
{
  // Reverse with step=-2: [9:0:-2] → 9, 7, 5, 3, 1
  std::array<float, 10> data;
  for (int i = 0; i < 10; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  auto dst = cle::slice(src, { cle::Slice(9, 0, -2) });

  EXPECT_EQ(dst->width(), 5);

  std::array<float, 5> result;
  dst->readTo(result.data());

  EXPECT_EQ(result[0], 9.0f);
  EXPECT_EQ(result[1], 7.0f);
  EXPECT_EQ(result[2], 5.0f);
  EXPECT_EQ(result[3], 3.0f);
  EXPECT_EQ(result[4], 1.0f);
}

// ─────────────────────────────────────────────────────────────────────────────
// S_() convenience constructors
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_s_convenience)
{
  std::array<float, 5 * 5> data;
  for (int i = 0; i < 5 * 5; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Use S_() convenience functions
  auto dst = cle::slice(src, { cle::S_(1, 4), cle::S_(2, 5) });

  EXPECT_EQ(dst->width(), 3);
  EXPECT_EQ(dst->height(), 3);

  std::array<float, 3 * 3> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 2; y < 5; y++)
  {
    for (int x = 1; x < 4; x++)
    {
      int src_idx = y * 5 + x;
      EXPECT_EQ(result[idx], data[src_idx]);
      idx++;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Error / exception tests
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, slice_null_source)
{
  cle::Array::Pointer null_src = nullptr;
  EXPECT_THROW(cle::slice(null_src, { cle::Slice() }), std::invalid_argument);
}

TEST_P(TestSlicing, slice_too_many_axes)
{
  std::array<float, 8> data = {};
  auto                 src = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  EXPECT_THROW(cle::slice(src, { cle::Slice(), cle::Slice(), cle::Slice(), cle::Slice() }), std::invalid_argument);
}

TEST_P(TestSlicing, slice_zero_step)
{
  std::array<float, 10> data = {};
  auto                  src = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  EXPECT_THROW(cle::slice(src, { cle::Slice(0, 5, 0) }), std::invalid_argument);
}

TEST_P(TestSlicing, slice_index_out_of_range)
{
  std::array<float, 5> data = {};
  auto                 src = cle::Array::create(5, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  EXPECT_THROW(cle::slice(src, { cle::Slice(10) }), std::out_of_range);
  EXPECT_THROW(cle::slice(src, { cle::Slice(-6) }), std::out_of_range);
}

TEST_P(TestSlicing, slice_empty_selection)
{
  std::array<float, 10> data = {};
  auto                  src = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // [5:2] with positive step produces empty range
  EXPECT_THROW(cle::slice(src, { cle::Slice(5, 2) }), std::out_of_range);
}

// ─────────────────────────────────────────────────────────────────────────────
// Paste tests - basic contiguous
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, paste_full_axis)
{
  // Paste a full array into a destination of the same size
  std::array<float, 2 * 3 * 4> src_data;
  for (int i = 0; i < 2 * 3 * 4; i++)
  {
    src_data[i] = static_cast<float>(i + 100);
  }

  std::array<float, 2 * 3 * 4> dst_data = {};
  auto                         src = cle::Array::create(2, 3, 4, 3, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto                         dst = cle::Array::create(2, 3, 4, 3, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(), cle::Slice(), cle::Slice() });

  std::array<float, 2 * 3 * 4> result;
  dst->readTo(result.data());

  for (int i = 0; i < 2 * 3 * 4; i++)
  {
    EXPECT_EQ(result[i], src_data[i]);
  }
}

TEST_P(TestSlicing, paste_subregion_contiguous)
{
  // Paste a small (3×3) block into a larger (6×6) destination at [1:4, 1:4]
  constexpr int dst_w = 6, dst_h = 6;
  constexpr int src_w = 3, src_h = 3;

  std::array<float, src_w * src_h> src_data;
  for (int i = 0; i < src_w * src_h; i++)
  {
    src_data[i] = static_cast<float>(i + 1);
  }

  // Initialize dst with zeros
  std::array<float, dst_w * dst_h> dst_data = {};

  auto src = cle::Array::create(src_w, src_h, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dst_w, dst_h, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(1, 4), cle::Slice(1, 4) });

  std::array<float, dst_w * dst_h> result;
  dst->readTo(result.data());

  // Verify that only [1:4, 1:4] region was written
  for (int y = 0; y < dst_h; y++)
  {
    for (int x = 0; x < dst_w; x++)
    {
      int dst_idx = y * dst_w + x;
      if (x >= 1 && x < 4 && y >= 1 && y < 4)
      {
        int src_idx = (y - 1) * src_w + (x - 1);
        EXPECT_EQ(result[dst_idx], src_data[src_idx]);
      }
      else
      {
        EXPECT_EQ(result[dst_idx], 0.0f);
      }
    }
  }
}

TEST_P(TestSlicing, paste_3d_contiguous)
{
  // Paste a (2×2×2) block into a (4×4×4) destination at [1:3, 1:3, 1:3]
  constexpr int sw = 2, sh = 2, sd = 2;
  constexpr int dw = 4, dh = 4, dd = 4;

  std::array<float, sw * sh * sd> src_data;
  for (int i = 0; i < sw * sh * sd; i++)
  {
    src_data[i] = static_cast<float>(i + 10);
  }

  std::array<float, dw * dh * dd> dst_data = {};
  auto                            src = cle::Array::create(sw, sh, sd, 3, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto                            dst = cle::Array::create(dw, dh, dd, 3, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(1, 3), cle::Slice(1, 3), cle::Slice(1, 3) });

  std::array<float, dw * dh * dd> result;
  dst->readTo(result.data());

  for (int z = 0; z < dd; z++)
  {
    for (int y = 0; y < dh; y++)
    {
      for (int x = 0; x < dw; x++)
      {
        int dst_idx = z * dh * dw + y * dw + x;
        if (x >= 1 && x < 3 && y >= 1 && y < 3 && z >= 1 && z < 3)
        {
          int src_idx = (z - 1) * sh * sw + (y - 1) * sw + (x - 1);
          EXPECT_EQ(result[dst_idx], src_data[src_idx]);
        }
        else
        {
          EXPECT_EQ(result[dst_idx], 0.0f);
        }
      }
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Paste tests - strided (with step)
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, paste_strided_1d)
{
  // Paste 5 elements into every-other position of a 10-element array: [0:10:2]
  constexpr int src_w = 5;
  constexpr int dst_w = 10;

  std::array<float, src_w> src_data = { 10, 20, 30, 40, 50 };
  std::array<float, dst_w> dst_data = {};

  auto src = cle::Array::create(src_w, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dst_w, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(0, 10, 2) });

  std::array<float, dst_w> result;
  dst->readTo(result.data());

  // Even positions should have src values, odd positions should remain 0
  for (int i = 0; i < dst_w; i++)
  {
    if (i % 2 == 0)
    {
      EXPECT_EQ(result[i], src_data[i / 2]);
    }
    else
    {
      EXPECT_EQ(result[i], 0.0f);
    }
  }
}

TEST_P(TestSlicing, paste_strided_2d)
{
  // Paste a (3×3) into every-other position of a (6×6): [0:6:2, 0:6:2]
  constexpr int sw = 3, sh = 3;
  constexpr int dw = 6, dh = 6;

  std::array<float, sw * sh> src_data;
  for (int i = 0; i < sw * sh; i++)
  {
    src_data[i] = static_cast<float>(i + 1);
  }

  std::array<float, dw * dh> dst_data = {};

  auto src = cle::Array::create(sw, sh, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dw, dh, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(0, 6, 2), cle::Slice(0, 6, 2) });

  std::array<float, dw * dh> result;
  dst->readTo(result.data());

  for (int y = 0; y < dh; y++)
  {
    for (int x = 0; x < dw; x++)
    {
      int dst_idx = y * dw + x;
      if (x % 2 == 0 && y % 2 == 0)
      {
        int src_idx = (y / 2) * sw + (x / 2);
        EXPECT_EQ(result[dst_idx], src_data[src_idx]);
      }
      else
      {
        EXPECT_EQ(result[dst_idx], 0.0f);
      }
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Paste tests - with index slices (single position)
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, paste_single_element)
{
  // Paste a single element into a specific position [2, 3, 1]
  constexpr int dw = 5, dh = 5, dd = 3;

  std::array<float, 1>            src_data = { 42.0f };
  std::array<float, dw * dh * dd> dst_data = {};

  auto src = cle::Array::create(1, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dw, dh, dd, 3, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(2), cle::Slice(3), cle::Slice(1) });

  std::array<float, dw * dh * dd> result;
  dst->readTo(result.data());

  int target_idx = 1 * dh * dw + 3 * dw + 2;
  EXPECT_EQ(result[target_idx], 42.0f);

  // Verify all other positions are still 0
  for (int i = 0; i < dw * dh * dd; i++)
  {
    if (i != target_idx)
    {
      EXPECT_EQ(result[i], 0.0f);
    }
  }
}

TEST_P(TestSlicing, paste_row_into_3d)
{
  // Paste a row (1D) into a 3D array at a specific z and y: [:, 2, 1]
  constexpr int dw = 4, dh = 5, dd = 3;

  std::array<float, dw>           src_data = { 10, 20, 30, 40 };
  std::array<float, dw * dh * dd> dst_data = {};

  auto src = cle::Array::create(dw, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dw, dh, dd, 3, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(), cle::Slice(2), cle::Slice(1) });

  std::array<float, dw * dh * dd> result;
  dst->readTo(result.data());

  for (int x = 0; x < dw; x++)
  {
    int idx = 1 * dh * dw + 2 * dw + x;
    EXPECT_EQ(result[idx], src_data[x]);
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Paste tests - different data types
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, paste_uint8)
{
  constexpr int sw = 2, sh = 2;
  constexpr int dw = 4, dh = 4;

  std::array<uint8_t, sw * sh> src_data = { 10, 20, 30, 40 };
  std::array<uint8_t, dw * dh> dst_data = {};

  auto src = cle::Array::create(sw, sh, 1, 2, cle::dType::UINT8, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dw, dh, 1, 2, cle::dType::UINT8, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(1, 3), cle::Slice(1, 3) });

  std::array<uint8_t, dw * dh> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 1; y < 3; y++)
  {
    for (int x = 1; x < 3; x++)
    {
      int dst_idx = y * dw + x;
      EXPECT_EQ(result[dst_idx], src_data[idx]);
      idx++;
    }
  }
}

TEST_P(TestSlicing, paste_int32)
{
  constexpr int sw = 2, sh = 3;
  constexpr int dw = 5, dh = 5;

  std::array<int32_t, sw * sh> src_data = { -5, -4, -3, -2, -1, 0 };
  std::array<int32_t, dw * dh> dst_data = {};

  auto src = cle::Array::create(sw, sh, 1, 2, cle::dType::INT32, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dw, dh, 1, 2, cle::dType::INT32, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(src, dst, { cle::Slice(2, 4), cle::Slice(1, 4) });

  std::array<int32_t, dw * dh> result;
  dst->readTo(result.data());

  int idx = 0;
  for (int y = 1; y < 4; y++)
  {
    for (int x = 2; x < 4; x++)
    {
      int dst_idx = y * dw + x;
      EXPECT_EQ(result[dst_idx], src_data[idx]);
      idx++;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Paste tests - convenience overload
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, paste_overload_xyz)
{
  constexpr int sw = 2, sh = 2, sd = 2;
  constexpr int dw = 4, dh = 4, dd = 4;

  std::array<float, sw * sh * sd> src_data;
  for (int i = 0; i < sw * sh * sd; i++)
  {
    src_data[i] = static_cast<float>(i + 100);
  }

  std::array<float, dw * dh * dd> dst_data = {};

  auto src = cle::Array::create(sw, sh, sd, 3, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(dw, dh, dd, 3, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  // Use the overload: paste(src, dst, x_slice, y_slice, z_slice)
  cle::paste(src, dst, cle::Slice(1, 3), cle::Slice(1, 3), cle::Slice(1, 3));

  std::array<float, dw * dh * dd> result;
  dst->readTo(result.data());

  for (int z = 1; z < 3; z++)
  {
    for (int y = 1; y < 3; y++)
    {
      for (int x = 1; x < 3; x++)
      {
        int dst_idx = z * dh * dw + y * dw + x;
        int src_idx = (z - 1) * sh * sw + (y - 1) * sw + (x - 1);
        EXPECT_EQ(result[dst_idx], src_data[src_idx]);
      }
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Paste tests - error cases
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, paste_null_source)
{
  std::array<float, 4> dst_data = {};
  auto                 dst = cle::Array::create(2, 2, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::Array::Pointer null_src = nullptr;
  EXPECT_THROW(cle::paste(null_src, dst, { cle::Slice() }), std::invalid_argument);
}

TEST_P(TestSlicing, paste_null_destination)
{
  std::array<float, 4> src_data = { 1, 2, 3, 4 };
  auto                 src = cle::Array::create(2, 2, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);

  cle::Array::Pointer null_dst = nullptr;
  EXPECT_THROW(cle::paste(src, null_dst, { cle::Slice() }), std::invalid_argument);
}

TEST_P(TestSlicing, paste_mismatched_dtype)
{
  std::array<float, 4>   src_data = { 1, 2, 3, 4 };
  std::array<uint8_t, 4> dst_data = {};

  auto src = cle::Array::create(2, 2, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(2, 2, 1, 2, cle::dType::UINT8, cle::mType::BUFFER, dst_data.data(), device);

  EXPECT_THROW(cle::paste(src, dst, { cle::Slice() }), std::invalid_argument);
}

TEST_P(TestSlicing, paste_mismatched_shape)
{
  // Source is 3×3 but target region is 2×2 — should throw
  std::array<float, 9>  src_data = {};
  std::array<float, 25> dst_data = {};

  auto src = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  EXPECT_THROW(cle::paste(src, dst, { cle::Slice(1, 3), cle::Slice(1, 3) }), std::invalid_argument);
}

TEST_P(TestSlicing, paste_too_many_axes)
{
  std::array<float, 8> src_data = {};
  std::array<float, 8> dst_data = {};

  auto src = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, src_data.data(), device);
  auto dst = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  EXPECT_THROW(cle::paste(src, dst, { cle::Slice(), cle::Slice(), cle::Slice(), cle::Slice() }), std::invalid_argument);
}

// ─────────────────────────────────────────────────────────────────────────────
// Roundtrip: slice then paste (and vice versa)
// ─────────────────────────────────────────────────────────────────────────────

TEST_P(TestSlicing, roundtrip_slice_then_paste)
{
  // Extract a sub-region with slice, then paste it back into a zeroed array
  constexpr int w = 6, h = 6;

  std::array<float, w * h> data;
  for (int i = 0; i < w * h; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(w, h, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1:4, 2:5]
  std::vector<cle::Slice> slices = { cle::Slice(1, 4), cle::Slice(2, 5) };
  auto                    sub = cle::slice(src, slices);

  EXPECT_EQ(sub->width(), 3);
  EXPECT_EQ(sub->height(), 3);

  // Paste back into a fresh destination at the same position
  std::array<float, w * h> dst_data = {};
  auto                     dst = cle::Array::create(w, h, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(sub, dst, slices);

  std::array<float, w * h> result;
  dst->readTo(result.data());

  // Only the [1:4, 2:5] region should have been written
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      int idx = y * w + x;
      if (x >= 1 && x < 4 && y >= 2 && y < 5)
      {
        EXPECT_EQ(result[idx], data[idx]);
      }
      else
      {
        EXPECT_EQ(result[idx], 0.0f);
      }
    }
  }
}

TEST_P(TestSlicing, roundtrip_strided_slice_then_paste)
{
  // Slice with step, then paste back at the same strided positions
  constexpr int w = 10;

  std::array<float, w> data;
  for (int i = 0; i < w; i++)
  {
    data[i] = static_cast<float>(i);
  }

  auto src = cle::Array::create(w, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, data.data(), device);

  // Slice [1:9:2] → elements at 1, 3, 5, 7
  std::vector<cle::Slice> slices = { cle::Slice(1, 9, 2) };
  auto                    sub = cle::slice(src, slices);

  EXPECT_EQ(sub->width(), 4);

  std::array<float, 4> sub_result;
  sub->readTo(sub_result.data());
  EXPECT_EQ(sub_result[0], 1.0f);
  EXPECT_EQ(sub_result[1], 3.0f);
  EXPECT_EQ(sub_result[2], 5.0f);
  EXPECT_EQ(sub_result[3], 7.0f);

  // Paste back at the same strided positions
  std::array<float, w> dst_data = {};
  auto                 dst = cle::Array::create(w, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  cle::paste(sub, dst, slices);

  std::array<float, w> result;
  dst->readTo(result.data());

  for (int i = 0; i < w; i++)
  {
    if (i == 1 || i == 3 || i == 5 || i == 7)
    {
      EXPECT_EQ(result[i], data[i]);
    }
    else
    {
      EXPECT_EQ(result[i], 0.0f);
    }
  }
}

TEST_P(TestSlicing, paste_multiple_regions)
{
  // Paste different sub-arrays into non-overlapping regions of a destination
  constexpr int dw = 8, dh = 1;

  std::array<float, 3>  src1_data = { 10, 20, 30 };
  std::array<float, 3>  src2_data = { 40, 50, 60 };
  std::array<float, dw> dst_data = {};

  auto src1 = cle::Array::create(3, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, src1_data.data(), device);
  auto src2 = cle::Array::create(3, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, src2_data.data(), device);
  auto dst = cle::Array::create(dw, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, dst_data.data(), device);

  // Paste src1 at [0:3], src2 at [5:8]
  cle::paste(src1, dst, { cle::Slice(0, 3) });
  cle::paste(src2, dst, { cle::Slice(5, 8) });

  std::array<float, dw> result;
  dst->readTo(result.data());

  EXPECT_EQ(result[0], 10.0f);
  EXPECT_EQ(result[1], 20.0f);
  EXPECT_EQ(result[2], 30.0f);
  EXPECT_EQ(result[3], 0.0f);
  EXPECT_EQ(result[4], 0.0f);
  EXPECT_EQ(result[5], 40.0f);
  EXPECT_EQ(result[6], 50.0f);
  EXPECT_EQ(result[7], 60.0f);
}

INSTANTIATE_TEST_SUITE_P(TestSlicing, TestSlicing, ::testing::ValuesIn(getParameters()));
