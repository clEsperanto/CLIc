#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestHessianGaussEigenvalues : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 11 * 11 * 1> input = { 
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0
  };
  std::array<float, 11 * 11 * 1> valid = {2.80377432e-03,  5.15507488e-03,  8.01434834e-03, 1.07877254e-02,  1.28035750e-02,  1.35410940e-02, 1.28035750e-02,  1.07877254e-02,  8.01434834e-03, 5.15507488e-03,  2.80377408e-03,
6.33742381e-03,  1.08882710e-02,  1.57999620e-02, 2.00331286e-02,  2.28355490e-02,  2.38104817e-02, 2.28355490e-02,  2.00331286e-02,  1.57999620e-02, 1.08882710e-02,  6.33742381e-03,
1.09319249e-02,  1.68837775e-02,  2.15000007e-02, 2.36747041e-02,  2.39554476e-02,  2.37844288e-02, 2.39554476e-02,  2.36747041e-02,  2.14999989e-02, 1.68837793e-02,  1.09319249e-02,
1.47780655e-02,  1.94353256e-02,  1.88947730e-02, 1.27731506e-02,  4.42769844e-03,  2.56299973e-06, 4.42769844e-03,  1.27731506e-02,  1.88947730e-02, 1.94353275e-02,  1.47780655e-02,
1.66362859e-02,  1.80286150e-02,  1.01901442e-02, -5.19440044e-03, -2.14566216e-02, -3.01977620e-02, -2.14566216e-02, -5.19440044e-03,  1.01901442e-02, 1.80286132e-02,  1.66362859e-02,
1.70330275e-02,  1.66780371e-02,  5.62216528e-03, -1.25080058e-02, -2.82402784e-02, -3.42185497e-02, -2.82402784e-02, -1.25080058e-02,  5.62216714e-03, 1.66780390e-02,  1.70330275e-02,
1.66362859e-02,  1.80286169e-02,  1.01901442e-02, -5.19439951e-03, -2.14566216e-02, -3.01977620e-02, -2.14566216e-02, -5.19440044e-03,  1.01901442e-02, 1.80286169e-02,  1.66362859e-02,
1.47780664e-02,  1.94353275e-02,  1.88947730e-02, 1.27731506e-02,  4.42769844e-03,  2.56299973e-06, 4.42769844e-03,  1.27731506e-02,  1.88947748e-02, 1.94353275e-02,  1.47780655e-02,
1.09319240e-02,  1.68837793e-02,  2.15000007e-02, 2.36747041e-02,  2.39554476e-02,  2.37844288e-02, 2.39554476e-02,  2.36747041e-02,  2.14999989e-02, 1.68837793e-02,  1.09319240e-02,
6.33742381e-03,  1.08882710e-02,  1.57999620e-02, 2.00331286e-02,  2.28355490e-02,  2.38104817e-02, 2.28355490e-02,  2.00331286e-02,  1.57999620e-02, 1.08882710e-02,  6.33742381e-03,
2.80377432e-03,  5.15507488e-03,  8.01434834e-03, 1.07877254e-02,  1.28035750e-02,  1.35410940e-02, 1.28035750e-02,  1.07877254e-02,  8.01434834e-03, 5.15507488e-03,  2.80377408e-03};
  std::array<float, 11 * 11 * 1> output;

};

TEST_P(TestHessianGaussEigenvalues, execute2D)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(11, 11, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::hessian_gaussian_eigenvalues_func(device, gpu_input, nullptr, nullptr, nullptr, 2.0F);

  gpu_output.front()->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
     EXPECT_NEAR(output[i], valid[i], 0.0001);
  }
}


std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestHessianGaussEigenvalues, ::testing::ValuesIn(getParameters()));


