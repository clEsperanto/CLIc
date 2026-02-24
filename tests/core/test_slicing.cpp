#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestSlicing : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
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

  EXPECT_EQ(dst->width(), 1);   // X dimension collapsed
  EXPECT_EQ(dst->height(), 4);  // Y dimension full
  EXPECT_EQ(dst->depth(), 5);   // Z dimension full

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

  EXPECT_EQ(dst->width(), 3);   // X dimension full
  EXPECT_EQ(dst->height(), 4);  // Y dimension full
  EXPECT_EQ(dst->depth(), 1);   // Z dimension collapsed
  EXPECT_EQ(dst->dim(), 2); // Should be 2D

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

  EXPECT_EQ(dst->width(), 1);   // X dimension collapsed
  EXPECT_EQ(dst->height(), 4);  // Y dimension full
  EXPECT_EQ(dst->depth(), 1);   // Z dimension collapsed

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

  // Slice [:7] — all but last 3 elements
  auto dst = cle::slice(src, { cle::Slice(std::nullopt, 7) });

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

INSTANTIATE_TEST_SUITE_P(TestSlicing, TestSlicing, ::testing::ValuesIn(getParameters()));
