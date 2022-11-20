#include "cleExecuteSeparableKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMemory.hpp"
#include "cleSeparableKernel.hpp"

namespace cle
{

ExecuteSeparableKernel::ExecuteSeparableKernel(const ProcessorPointer & device)
  : Operation(device)
{}

auto
ExecuteSeparableKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
ExecuteSeparableKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
ExecuteSeparableKernel::SetKernelSize(const int & radius_x, const int & radius_y, const int & radius_z) -> void
{
  this->kernel_size_ = { radius_x, radius_y, radius_z };
}

auto
ExecuteSeparableKernel::SetSigma(const float & sigma_x, const float & sigma_y, const float & sigma_z) -> void
{
  this->sigma_ = { sigma_x, sigma_y, sigma_z };
}

auto
ExecuteSeparableKernel::Execute() -> void
{
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  // * check src/dst is float type ?

  auto temp1 = Memory::AllocateMemory(*dst);
  auto temp2 = Memory::AllocateMemory(*dst);

  CopyKernel      copy(this->GetDevice());
  SeparableKernel kernel(this->GetDevice());
  kernel.SetSource(this->GetName(), this->GetSource());
  if (dst->Shape()[0] > 1 && this->sigma_[0] > 0)
  {
    kernel.SetInput(*src);
    kernel.SetOutput(temp1);
    kernel.SetSigma(this->sigma_[0]);
    kernel.SetSize(this->kernel_size_[0]);
    kernel.SetDimension(0);
    kernel.Execute();
  }
  else
  {
    copy.SetInput(*src);
    copy.SetOutput(temp1);
    copy.Execute();
    // src->CopyDataTo (temp1); // * Byte-wise copy ?
  }
  if (dst->Shape()[1] > 1 && this->sigma_[1] > 0)
  {
    kernel.SetInput(temp1);
    kernel.SetOutput(temp2);
    kernel.SetSigma(this->sigma_[1]);
    kernel.SetSize(this->kernel_size_[1]);
    kernel.SetDimension(1);
    kernel.Execute();
  }
  else
  {
    copy.SetInput(temp1);
    copy.SetOutput(temp2);
    copy.Execute();
    // temp1.CopyDataTo (temp2); // * Byte-wise copy ?
  }
  if (dst->Shape()[2] > 1 && this->sigma_[2] > 0)
  {
    kernel.SetInput(temp2);
    kernel.SetOutput(*dst);
    kernel.SetSigma(this->sigma_[2]);
    kernel.SetSize(this->kernel_size_[2]);
    kernel.SetDimension(2);
    kernel.Execute();
  }
  else
  {
    copy.SetInput(temp2);
    copy.SetOutput(*dst);
    copy.Execute();
    // temp2.CopyDataTo (*dst); // * Byte-wise copy ?
  }
}

} // namespace cle
