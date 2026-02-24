
#include "cle.hpp"
#include "execution.hpp"

#include "test_utils.hpp"
#include <gtest/gtest.h>

class TestEvaluate : public ::testing::TestWithParam<std::string>
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

TEST_P(TestEvaluate, addTwoArrays)
{


  auto a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto b = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(3);
  b->fill(5);

  cle::evaluate(device, "a + b", { a, b }, out);

  std::vector<float> result(10);
  out->readTo(result.data());
  for (int i = 0; i < 10; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 8.0f);
  }
}

TEST_P(TestEvaluate, expressionWithScalar)
{


  auto a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto b = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(2);
  b->fill(3);

  // expression: a + b * s  =>  2 + 3 * 2.5 = 9.5
  cle::evaluate(device, "a + b * s", { a, b, 2.5f }, out);

  std::vector<float> result(10);
  out->readTo(result.data());
  for (int i = 0; i < 10; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 9.5f);
  }
}

TEST_P(TestEvaluate, mathFunctions)
{


  auto a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(4.0f);

  // sqrt(4) = 2
  cle::evaluate(device, "sqrt(a)", { a }, out);

  std::vector<float> result(10);
  out->readTo(result.data());
  for (int i = 0; i < 10; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 2.0f);
  }
}

TEST_P(TestEvaluate, complexExpression)
{


  auto a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto b = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(2.0f);
  b->fill(3.0f);

  // (a * a + b * b) = 4 + 9 = 13
  cle::evaluate(device, "a * a + b * b", { a, b }, out);

  std::vector<float> result(10);
  out->readTo(result.data());
  for (int i = 0; i < 10; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 13.0f);
  }
}

TEST_P(TestEvaluate, array2D)
{


  auto a = cle::Array::create(5, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto b = cle::Array::create(5, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(5, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(1.0f);
  b->fill(2.0f);

  cle::evaluate(device, "a - b", { a, b }, out);

  std::vector<float> result(20);
  out->readTo(result.data());
  for (int i = 0; i < 20; i++)
  {
    EXPECT_FLOAT_EQ(result[i], -1.0f);
  }
}

TEST_P(TestEvaluate, array3D)
{


  auto a = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(5.0f);

  // a * 2.0f (literal in expression)
  cle::evaluate(device, "a * 2.0f", { a }, out);

  std::vector<float> result(24);
  out->readTo(result.data());
  for (int i = 0; i < 24; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 10.0f);
  }
}

TEST_P(TestEvaluate, mixedDtypes)
{


  auto a = cle::Array::create(10, 1, 1, 1, cle::dType::UINT8, cle::mType::BUFFER, device);
  auto b = cle::Array::create(10, 1, 1, 1, cle::dType::INT16, cle::mType::BUFFER, device);
  auto out = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  std::vector<uint8_t> a_data(10, 100);
  std::vector<int16_t> b_data(10, 50);
  a->writeFrom(a_data.data());
  b->writeFrom(b_data.data());

  cle::evaluate(device, "a + b", { a, b }, out);

  std::vector<float> result(10);
  out->readTo(result.data());
  for (int i = 0; i < 10; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 150.0f);
  }
}

TEST_P(TestEvaluate, multipleScalars)
{


  auto a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto out = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  a->fill(1.0f);

  // a * gain + offset  =>  1 * 3 + 10 = 13
  cle::evaluate(device, "a * gain + offset", { a, 3.0f, 10.0f }, out);

  std::vector<float> result(10);
  out->readTo(result.data());
  for (int i = 0; i < 10; i++)
  {
    EXPECT_FLOAT_EQ(result[i], 13.0f);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestEvaluate, ::testing::ValuesIn(getParameters()));
