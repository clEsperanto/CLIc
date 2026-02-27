#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestSetCreateOperations : public ::testing::TestWithParam<std::tuple<std::string, int>>
{
protected:
  std::array<float, 10 * 5 * 3>   output_float;
  std::array<int16_t, 5 * 5 * 1>  output_int16;
  std::array<int32_t, 3 * 3 * 2>  output_int32;
  std::array<uint32_t, 5 * 3 * 2> output_uint32;

  std::array<float, 10 * 5 * 3>  input_float;
  std::array<int16_t, 5 * 5 * 1> input_int16;
  std::array<int32_t, 3 * 3 * 2> input_int32;
  std::array<float, 5 * 3 * 2>   input_pixelindex;

  virtual void
  SetUp()
  {
    // Initialize float inputs with random values
    for (auto it = input_float.begin(); it != input_float.end(); ++it)
    {
      *it = static_cast<float>((int)rand() % 10);
    }

    // Initialize int16 inputs with constant value
    std::fill(input_int16.begin(), input_int16.end(), static_cast<int16_t>(3));

    // Initialize int32 inputs with some values
    input_int32 = { 0, 0, 0, 3, 4, 3, 3, 4, 3, 3, 4, 3, 3, 4, 3, 3, 4, 3 };

    // Initialize pixelindex input with random binary values
    std::fill(input_pixelindex.begin(), input_pixelindex.end(), 0);
    for (auto it = input_pixelindex.begin(); it != input_pixelindex.end(); ++it)
    {
      *it = static_cast<float>(rand() % 2);
    }
  }
};

TEST_P(TestSetCreateOperations, execute)
{
  auto [backend, operation_type] = GetParam();
  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  switch (operation_type)
  {
    case 0: // set_ramp_x
    {
      std::array<int32_t, 3 * 3 * 2> expected_x = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 };
      auto                           gpu_input = cle::Array::create(3, 3, 2, 3, cle::dType::INT32, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_int32.data());
      cle::tier1::set_ramp_x_func(device, gpu_input);
      gpu_input->readTo(output_int32.data());
      for (int i = 0; i < output_int32.size(); i++)
      {
        EXPECT_EQ(output_int32[i], expected_x[i]);
      }
      break;
    }
    case 1: // set_ramp_y
    {
      std::array<int32_t, 3 * 3 * 2> expected_y = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2 };
      auto                           gpu_input = cle::Array::create(3, 3, 2, 3, cle::dType::INT32, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_int32.data());
      cle::tier1::set_ramp_y_func(device, gpu_input);
      gpu_input->readTo(output_int32.data());
      for (int i = 0; i < output_int32.size(); i++)
      {
        EXPECT_EQ(output_int32[i], expected_y[i]);
      }
      break;
    }
    case 2: // set_ramp_z
    {
      std::array<int32_t, 3 * 3 * 2> expected_z = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
      auto                           gpu_input = cle::Array::create(3, 3, 2, 3, cle::dType::INT32, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_int32.data());
      cle::tier1::set_ramp_z_func(device, gpu_input);
      gpu_input->readTo(output_int32.data());
      for (int i = 0; i < output_int32.size(); i++)
      {
        EXPECT_EQ(output_int32[i], expected_z[i]);
      }
      break;
    }
    case 3: // set_row
    {
      auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_float.data());
      cle::tier1::set_row_func(device, gpu_input, 1, 100);
      gpu_input->readTo(output_float.data());

      // Verify that row 1 is set to 100
      for (int z = 0; z < 3; z++)
      {
        for (int y = 0; y < 5; y++)
        {
          for (int x = 0; x < 10; x++)
          {
            int    idx = z * 50 + y * 10 + x;
            size_t row_index = y;
            if (row_index == 1)
            {
              EXPECT_EQ(output_float[idx], 100.0f);
            }
            else
            {
              EXPECT_EQ(output_float[idx], input_float[idx]);
            }
          }
        }
      }
      break;
    }
    case 4: // set_column
    {
      auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_float.data());
      cle::tier1::set_column_func(device, gpu_input, 1, 100);
      gpu_input->readTo(output_float.data());

      // Verify that column 1 is set to 100
      for (int z = 0; z < 3; z++)
      {
        for (int y = 0; y < 5; y++)
        {
          for (int x = 0; x < 10; x++)
          {
            int idx = z * 50 + y * 10 + x;
            if (x == 1)
            {
              EXPECT_EQ(output_float[idx], 100.0f);
            }
            else
            {
              EXPECT_EQ(output_float[idx], input_float[idx]);
            }
          }
        }
      }
      break;
    }
    case 5: // set_plane
    {
      std::array<float, 5 * 5 * 2> expected_plane = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                                      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
      std::array<float, 5 * 5 * 2> input_plane = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
      std::array<float, 5 * 5 * 2> output_plane;
      auto                         gpu_input = cle::Array::create(5, 5, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_plane.data());
      cle::tier1::set_plane_func(device, gpu_input, 1, 4);
      gpu_input->readTo(output_plane.data());
      for (int i = 0; i < output_plane.size(); i++)
      {
        EXPECT_EQ(output_plane[i], expected_plane[i]);
      }
      break;
    }
    case 6: // set_image_borders
    {
      std::array<int16_t, 5 * 5 * 1> expected_border = { 4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4, 4 };
      auto                           gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::INT16, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_int16.data());
      cle::tier1::set_image_borders_func(device, gpu_input, 4);
      gpu_input->readTo(output_int16.data());
      for (int i = 0; i < output_int16.size(); i++)
      {
        EXPECT_EQ(output_int16[i], expected_border[i]);
      }
      break;
    }
    case 7: // set_nonzero_pixels_to_pixelindex
    {
      std::array<uint32_t, 5 * 3 * 2> expected_pixelindex;
      std::fill(expected_pixelindex.begin(), expected_pixelindex.end(), 0);
      auto it_input = input_pixelindex.begin();
      auto it_expected = expected_pixelindex.begin();
      for (; (it_input != input_pixelindex.end()) && (it_expected != expected_pixelindex.end()); ++it_input, ++it_expected)
      {
        if (*it_input != 0)
        {
          *it_expected = static_cast<uint32_t>((it_expected - expected_pixelindex.begin()) + 1);
        }
      }

      auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
      gpu_input->writeFrom(input_pixelindex.data());
      auto gpu_output = cle::tier1::set_nonzero_pixels_to_pixelindex_func(device, gpu_input, nullptr, 1);
      gpu_output->readTo(output_uint32.data());
      for (int i = 0; i < output_uint32.size(); i++)
      {
        EXPECT_EQ(output_uint32[i], expected_pixelindex[i]);
      }
      break;
    }
    default:
      GTEST_SKIP();
  }
}

std::vector<std::tuple<std::string, int>>
getSetCreateParameters()
{
  std::vector<std::tuple<std::string, int>> params;
  auto                                      backends = getParameters();
  for (const auto & backend : backends)
  {
    for (int i = 0; i < 8; i++)
    {
      params.push_back(std::make_tuple(backend, i));
    }
  }
  return params;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSetCreateOperations, ::testing::ValuesIn(getSetCreateParameters()));
