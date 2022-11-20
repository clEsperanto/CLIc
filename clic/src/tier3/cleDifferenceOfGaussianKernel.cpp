

#include "cleDifferenceOfGaussianKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"
#include "cleGaussianBlurKernel.hpp"
#include "cleMemory.hpp"

namespace cle
{

DifferenceOfGaussianKernel::DifferenceOfGaussianKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
DifferenceOfGaussianKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
DifferenceOfGaussianKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
DifferenceOfGaussianKernel::SetSigma1(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void
{
  this->sigma1_ = { sigma_x, sigma_y, sigma_z };
}

auto
DifferenceOfGaussianKernel::SetSigma2(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void
{
  this->sigma2_ = { sigma_x, sigma_y, sigma_z };
}

auto
DifferenceOfGaussianKernel::Execute() -> void
{
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto temp1 = Memory::AllocateMemory(this->GetDevice(), src->Shape(), src->GetDataType(), src->GetMemoryType());
  auto temp2 = Memory::AllocateMemory(this->GetDevice(), src->Shape(), src->GetDataType(), src->GetMemoryType());

  GaussianBlurKernel gaussian_1_kernel(this->GetDevice());
  gaussian_1_kernel.SetInput(*src);
  gaussian_1_kernel.SetOutput(temp1);
  gaussian_1_kernel.SetSigma(this->sigma1_[0], this->sigma1_[1], this->sigma1_[2]);
  gaussian_1_kernel.Execute();

  GaussianBlurKernel gaussian_2_kernel(this->GetDevice());
  gaussian_2_kernel.SetInput(*src);
  gaussian_2_kernel.SetOutput(temp2);
  gaussian_2_kernel.SetSigma(this->sigma2_[0], this->sigma2_[1], this->sigma2_[2]);
  gaussian_2_kernel.Execute();

  AddImagesWeightedKernel difference(this->GetDevice());
  difference.SetInput1(temp1);
  difference.SetInput2(temp2);
  difference.SetOutput(*dst);
  difference.SetFactor1(1);
  difference.SetFactor2(-1);
  difference.Execute();
}

} // namespace cle
