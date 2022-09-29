#include "cleGaussianBlurKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"
#include "cleMemory.hpp"

namespace cle
{

GaussianBlurKernel::GaussianBlurKernel(const ProcessorPointer & device)
  : Operation(device)
{
  std::string cl_header = {
#include "cle_gaussian_blur_separable.h"
  };
  this->SetSource("gaussian_blur_separable", cl_header);
}

auto
GaussianBlurKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
GaussianBlurKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
GaussianBlurKernel::SetSigma(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void
{
  this->sigma_ = { sigma_x, sigma_y, sigma_z };
}

auto
GaussianBlurKernel::Sigma2KernelSize(const std::array<float, 3> & sigmas) -> std::array<size_t, 3>
{
  const float           factor = 8.0;
  const float           rounding = 5.0;
  std::array<size_t, 3> kernel_size{};
  auto                  it_size(kernel_size.begin());
  auto                  size_end(kernel_size.end());
  auto                  it_sigma(sigmas.begin());
  auto                  sigma_end(sigmas.end());
  for (; (it_sigma != sigma_end) && (it_size != size_end); ++it_sigma, ++it_size)
  {
    *it_size = static_cast<size_t>(*it_sigma * factor + rounding);
    if (*it_size % 2 == 0)
    {
      *it_size += 1;
    }
  }
  return kernel_size;
}

auto
GaussianBlurKernel::Execute() -> void
{
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto kernel_size = Sigma2KernelSize(this->sigma_);

  ExecuteSeparableKernel kernel(this->GetDevice());
  kernel.SetSource(this->GetName(), this->GetSource());
  kernel.SetInput(*src);
  kernel.SetSigma(this->sigma_[0], this->sigma_[1], this->sigma_[2]);
  kernel.SetKernelSize(kernel_size[0], kernel_size[1], kernel_size[2]);

  if (dst->GetDataType() != FLOAT)
  {
    auto temp = Memory::AllocateMemory(this->GetDevice(), dst->Shape(), FLOAT, dst->GetMemoryType());
    kernel.SetOutput(temp);
    kernel.Execute();

    CopyKernel copy(this->GetDevice());
    copy.SetInput(temp);
    copy.SetOutput(*dst);
    copy.Execute();
  }
  else
  {
    kernel.SetOutput(*dst);
    kernel.Execute();
  }
}

} // namespace cle
