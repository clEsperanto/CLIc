

#include "cleMaximumBoxKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

MaximumBoxKernel::MaximumBoxKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_maximum_separable.h"
    };
    this->SetSource ("cle_maximum_separable", cl_header);
}

int
MaximumBoxKernel::Radius2KernelSize (const int &radius) const
{
    return radius * 2 + 1;
}

void
MaximumBoxKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MaximumBoxKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
MaximumBoxKernel::SetRadius (const int &radius_x, const int &radius_y, const int &radius_z)
{
    this->radius_ = { radius_x, radius_y, radius_z };
}

void
MaximumBoxKernel::Execute ()
{
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");

    // int nx = Radius2KernelSize (this->radius_[0]);
    // int ny = Radius2KernelSize (this->radius_[1]);
    // int nz = Radius2KernelSize (this->radius_[2]);

    // ExecuteSeparableKernel kernel (this->m_gpu);
    // kernel.SetKernelName (this->m_KernelName);
    // kernel.SetSources (this->m_Sources);
    // kernel.SetInput (*src);
    // kernel.SetOutput (*dst);
    // kernel.SetSigma (this->m_x, this->m_y, this->m_z);
    // kernel.SetKernelSize (nx, ny, nz);
    // kernel.Execute ();
}

} // namespace cle
