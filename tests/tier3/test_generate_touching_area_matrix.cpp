#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestGenerateTouchingAreaMatrix : public ::testing::TestWithParam<std::string>
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

TEST_P(TestGenerateTouchingAreaMatrix, execute)
{
  // 3D label image: width=6, height=2, depth=2
  // Slice z=0: [ [0,1,1,2,2,3], [0,1,1,2,2,3] ]
  // Slice z=1: [ [0,0,0,0,0,0], [0,0,0,0,0,0] ]
  // Touching boundary pixel counts (per direction, then symmetrised):
  //   label 0 touches label 1: 2 pixels (left edge of col 1, both rows) x2 slices = 4? No —
  //   only z=0 slice has non-zero labels:
  //     0↔1: col 0–1 border: 2 pixels (rows 0 and 1) × 1 slice → 2 each side → sum = 4? 
  //   Let's follow the Python reference directly:
  //   reference = [[0,6,4,2],[6,0,2,0],[4,2,0,2],[2,0,2,0]]
  // clang-format off
  std::array<uint32_t, 6 * 2 * 2> input = {
    // z = 0
    0, 1, 1, 2, 2, 3,
    0, 1, 1, 2, 2, 3,
    // z = 1
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
  };

  // Expected symmetric 4×4 touching-area matrix (labels 0..3)
  // row/col order: 0, 1, 2, 3
  std::array<uint32_t, 4 * 4> valid = {
    0, 6, 4, 2,
    6, 0, 2, 0,
    4, 2, 0, 2,
    2, 0, 2, 0
  };
  // clang-format on

  auto gpu_input = cle::Array::create(6, 2, 2, 3, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier3::generate_touching_area_matrix_func(device, gpu_input, nullptr);

  std::array<uint32_t, 4 * 4> output;
  gpu_output->readTo(output.data());

  for (int i = 0; i < static_cast<int>(output.size()); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName,
                         TestGenerateTouchingAreaMatrix,
                         ::testing::ValuesIn(getParameters()));
