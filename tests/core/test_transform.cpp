
#include "cle.hpp"
#include "transform.hpp"

#include "test_utils.hpp"
#include <gtest/gtest.h>

class TestTransform : public ::testing::TestWithParam<std::string>
{
protected:
  cle::transform::AffineTransform transform;

  void
  SetUp() override
  {
    transform = cle::transform::AffineTransform();
  }
};

TEST_P(TestTransform, toArray)
{
  transform.rotate(2, 90);
  auto                  array = cle::transform::AffineTransform::toArray(transform.getTranspose());
  std::array<float, 16> valid = { 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
  for (int i = 0; i < array.size(); i++)
  {
    EXPECT_EQ(array[i], valid[i]);
  }
}

TEST_P(TestTransform, translateX)
{
  transform.translate(1.0, 0.0, 0.0);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 3) = 1.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, translateY)
{
  transform.translate(0.0, 1.0, 0.0);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(1, 3) = 1.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, translateZ)
{
  transform.translate(0.0, 0.0, 1.0);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(2, 3) = 1.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, translateXYZ)
{
  transform.translate(1.0, 2.0, 3.0);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 3) = 1.0;
  expected(1, 3) = 2.0;
  expected(2, 3) = 3.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, rotateAlongX)
{
  transform.rotate(0, 90);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(1, 1) = 0;
  expected(1, 2) = -1;
  expected(2, 1) = 1;
  expected(2, 2) = 0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, rotateAlongY)
{
  transform.rotate(1, 90);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 0) = 0;
  expected(0, 2) = 1;
  expected(2, 0) = -1;
  expected(2, 2) = 0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, rotateAlongZ)
{
  transform.rotate(2, 90);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 0) = 0;
  expected(0, 1) = -1;
  expected(1, 0) = 1;
  expected(1, 1) = 0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, centerUndo)
{
  std::array<size_t, 3> shape = { 10, 20, 30 };
  transform.center(shape, true);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 3) = 5.0;
  expected(1, 3) = 10.0;
  expected(2, 3) = 15.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, centerNoUndo)
{
  std::array<size_t, 3> shape = { 10, 20, 30 };
  transform.center(shape, false);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 3) = -5.0;
  expected(1, 3) = -10.0;
  expected(2, 3) = -15.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, centerRotate)
{
  std::array<size_t, 3> shape = { 30, 20, 10 };

  transform.center(shape, false);
  transform.rotate(2, 90);
  transform.center(shape, true);

  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 0) = 0;
  expected(0, 1) = -1;
  expected(1, 0) = 1;
  expected(1, 1) = 0;
  expected(0, 3) = 25.0;
  expected(1, 3) = -5.0;
  expected(2, 3) = 0.0;
  ASSERT_EQ(transform.getMatrix(), expected);
}

TEST_P(TestTransform, getValueIdentity)
{
  ASSERT_EQ(transform.getValue(0, 0), 1.0);
  ASSERT_EQ(transform.getValue(1, 1), 1.0);
  ASSERT_EQ(transform.getValue(2, 2), 1.0);
  ASSERT_EQ(transform.getValue(3, 3), 1.0);
  ASSERT_EQ(transform.getValue(0, 3), 0.0);
  ASSERT_EQ(transform.getValue(2, 1), 0.0);
}

TEST_P(TestTransform, setValue)
{
  transform.setValue(0, 3, 1.5);
  transform.setValue(1, 1, 2.0);
  Eigen::Matrix4f expected = Eigen::Matrix4f::Identity();
  expected(0, 3) = 1.5;
  expected(1, 1) = 2.0;
  ASSERT_EQ(transform.getMatrix(), expected);
  ASSERT_EQ(transform.getValue(0, 3), 1.5);
  ASSERT_EQ(transform.getValue(1, 1), 2.0);
}

TEST_P(TestTransform, setValueUpdatesDerivedMatrices)
{
  transform.setValue(0, 0, 2.0);
  Eigen::Matrix4f expected_transpose = Eigen::Matrix4f::Identity();
  expected_transpose(0, 0) = 2.0;
  ASSERT_EQ(transform.getTranspose(), expected_transpose);
  Eigen::Matrix4f expected_inverse = Eigen::Matrix4f::Identity();
  expected_inverse(0, 0) = 0.5;
  ASSERT_EQ(transform.getInverse(), expected_inverse);
}

// TODO: test shear and deskew
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestTransform, ::testing::ValuesIn(getParameters()));
