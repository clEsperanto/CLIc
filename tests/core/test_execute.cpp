
#include "cle.hpp"
#include "execution.hpp"

#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestExecution : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestExecution, parameterType)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  // Test assignment of cle::Array::Pointer
  auto               ap = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::ParameterType pt1 = ap;
  ASSERT_TRUE(std::holds_alternative<cle::Array::Pointer>(pt1));

  // Test assignment of float
  float              fp = 0.5f;
  cle::ParameterType pt2 = fp;
  ASSERT_TRUE(std::holds_alternative<float>(pt2));

  // Test assignment of int
  int                ip = 2;
  cle::ParameterType pt3 = ip;
  ASSERT_TRUE(std::holds_alternative<int>(pt3));
}

TEST_P(TestExecution, parameterList)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto  src = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto  dst = cle::Array::create(3, 5, 10, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  int   slice = 2;
  float scalar = 5.0f;

  const cle::ParameterList params = { { "src", src }, { "dst", dst }, { "index", slice }, { "scalar", scalar } };

  // Test the items in cle::ParameterList

  ASSERT_EQ(params.size(), 4);
  ASSERT_TRUE(params[0].first == "src" && std::holds_alternative<cle::Array::Pointer>(params[0].second));
  ASSERT_TRUE(params[1].first == "dst" && std::holds_alternative<cle::Array::Pointer>(params[1].second));
  ASSERT_TRUE(params[2].first == "index" && std::holds_alternative<int>(params[2].second));
  ASSERT_TRUE(params[3].first == "scalar" && std::holds_alternative<float>(params[3].second));
}

TEST_P(TestExecution, rangeArray)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  size_t width = 5, height = 10, depth = 15;
  auto   dst = cle::Array::create(width, height, depth, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

  const cle::RangeArray range = { dst->width(), dst->height(), dst->depth() };

  // Test the elements in cle::RangeArray
  ASSERT_EQ(range.size(), 3);
  ASSERT_EQ(range[0], width);
  ASSERT_EQ(range[1], height);
  ASSERT_EQ(range[2], depth);
}

// TEST_P(TestExecution, constantList)
// {
//   int                     bins = 255;
//   cle::ConstantType       cts = bins;
//   const cle::ConstantList constants = { { "bins", cts } };

//   ASSERT_EQ(constants.size(), 1);
//   ASSERT_EQ(constants[0].first, "bins");
//   ASSERT_EQ(constants[0].second, bins);
// }

TEST_P(TestExecution, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto arr_a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto arr_b = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  arr_a->fill(-1);
  arr_b->fill(2);


  std::string name = "absolute";
  std::string source =
    R"(__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;
	__kernel void absolute( IMAGE_src_TYPE  src, IMAGE_dst_TYPE  dst )	{
	  const int x = get_global_id(0);
	  const int y = get_global_id(1);
	  const int z = get_global_id(2);
	  IMAGE_src_PIXEL_TYPE value = READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
	  if ( value < 0 ) {
	    value = -1 * value;
	  }
	  WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(value));
	})";

  execute(device, { name, source }, { { "src", arr_a }, { "dst", arr_b } }, { arr_a->width(), arr_a->height(), arr_a->depth() });


  std::vector<float> h_c(10);
  arr_b->readTo(h_c.data());
  for (int i = 0; i < h_c.size(); i++)
  {
    EXPECT_EQ(h_c[i], 1);
  }
}


TEST_P(TestExecution, executeNative)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto arr_a = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto arr_b = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto arr_c = cle::Array::create(10, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  arr_a->fill(1);
  arr_b->fill(2);

  std::string name = "add_arrays";
  std::string source =
    R"(__kernel void add_arrays(__global const float* a, __global const float* b, __global float* c, const unsigned int n) {
    int id = get_global_id(0);
    if (id < n) {
        c[id] = a[id] + b[id];
    }
})";

  native_execute(device,
                 { name, source },
                 { { "a", arr_a }, { "b", arr_b }, { "c", arr_c }, { "n", 10 } },
                 { arr_a->width(), arr_a->height(), arr_a->depth() },
                 { 1, 1, 1 });


  std::vector<float> h_c(10);
  arr_c->readTo(h_c.data());
  for (int i = 0; i < h_c.size(); i++)
  {
    EXPECT_EQ(h_c[i], 3);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestExecution, ::testing::ValuesIn(getParameters()));
