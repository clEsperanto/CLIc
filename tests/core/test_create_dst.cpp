#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestCreate : public ::testing::TestWithParam<std::string>
{
protected:
  cle::Array::Pointer output;
  cle::Array::Pointer input;
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

TEST_P(TestCreate, notNullPtr)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  output = cle::Array::create(3, 5, 10, 3, cle::dType::UINT16, cle::mType::BUFFER, device);

  cle::tier0::create_dst(input, output, 2, 3, 4, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_like(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_one(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_vector(input, output, 10, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_xy(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_yx(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_zy(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_yz(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_xz(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);

  cle::tier0::create_zx(input, output, cle::dType::UINT8);
  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 3);
  EXPECT_EQ(output->height(), 5);
  EXPECT_EQ(output->depth(), 10);
  EXPECT_EQ(output->dim(), 3);
  EXPECT_EQ(output->dimension(), 3);
  EXPECT_EQ(output->itemSize(), sizeof(uint16_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT16);
}

TEST_P(TestCreate, create_dst)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_dst(input, output, 2, 3, 4, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 2);
  EXPECT_EQ(output->height(), 3);
  EXPECT_EQ(output->depth(), 4);
  EXPECT_EQ(output->dim(), input->dim());
  EXPECT_EQ(output->dimension(), input->dimension());
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_dst(input, output, 2, 3, 4);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 2);
  EXPECT_EQ(output->height(), 3);
  EXPECT_EQ(output->depth(), 4);
  EXPECT_EQ(output->dim(), input->dim());
  EXPECT_EQ(output->dimension(), input->dimension());
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}


TEST_P(TestCreate, create_like)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_like(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->width());
  EXPECT_EQ(output->height(), input->height());
  EXPECT_EQ(output->depth(), input->depth());
  EXPECT_EQ(output->dim(), input->dim());
  EXPECT_EQ(output->dimension(), input->dimension());
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_like(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->width());
  EXPECT_EQ(output->height(), input->height());
  EXPECT_EQ(output->depth(), input->depth());
  EXPECT_EQ(output->dim(), input->dim());
  EXPECT_EQ(output->dimension(), input->dimension());
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_one)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_one(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 1);
  EXPECT_EQ(output->height(), 1);
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 1);
  EXPECT_EQ(output->dimension(), 1);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_one(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 1);
  EXPECT_EQ(output->height(), 1);
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 1);
  EXPECT_EQ(output->dimension(), 1);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_vector)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_vector(input, output, 10, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 10);
  EXPECT_EQ(output->height(), 1);
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 1);
  EXPECT_EQ(output->dimension(), 1);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_vector(input, output, 10);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), 10);
  EXPECT_EQ(output->height(), 1);
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 1);
  EXPECT_EQ(output->dimension(), 1);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_xy)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_xy(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->width());
  EXPECT_EQ(output->height(), input->height());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_xy(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->width());
  EXPECT_EQ(output->height(), input->height());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_yx)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_yx(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->height());
  EXPECT_EQ(output->height(), input->width());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_yx(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->height());
  EXPECT_EQ(output->height(), input->width());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_zy)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_zy(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->depth());
  EXPECT_EQ(output->height(), input->height());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_zy(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->depth());
  EXPECT_EQ(output->height(), input->height());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_yz)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_yz(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->height());
  EXPECT_EQ(output->height(), input->depth());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_yz(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->height());
  EXPECT_EQ(output->height(), input->depth());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_xz)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_xz(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->width());
  EXPECT_EQ(output->height(), input->depth());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_xz(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->width());
  EXPECT_EQ(output->height(), input->depth());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}

TEST_P(TestCreate, create_zx)
{

  input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier0::create_zx(input, output, cle::dType::UINT8);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->depth());
  EXPECT_EQ(output->height(), input->width());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), sizeof(uint8_t));
  EXPECT_EQ(output->dtype(), cle::dType::UINT8);

  output = nullptr;
  cle::tier0::create_zx(input, output);

  EXPECT_EQ(output->device(), input->device());
  EXPECT_EQ(output->width(), input->depth());
  EXPECT_EQ(output->height(), input->width());
  EXPECT_EQ(output->depth(), 1);
  EXPECT_EQ(output->dim(), 2);
  EXPECT_EQ(output->dimension(), 2);
  EXPECT_EQ(output->itemSize(), input->itemSize());
  EXPECT_EQ(output->dtype(), input->dtype());
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCreate, ::testing::ValuesIn(getParameters()));
