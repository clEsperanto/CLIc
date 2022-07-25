
#include "cleMeanSphereKernel.hpp"

namespace cle
{

MeanSphereKernel::MeanSphereKernel (const ProcessorPointer &device) : Operation (device, 5)
{
    std::string cl_header = {
#include "cle_mean_sphere.h"
    };
    this->SetSource ("cle_mean_sphere", cl_header);
}

int
MeanSphereKernel::Radius2KernelSize (const float &radius) const
{
    return static_cast<int> (radius) * 2 + 1;
}

void
MeanSphereKernel::SetRadius (const float &radius_x, const float &radius_y, const float &radius_z)
{
    int rx = Radius2KernelSize (radius_x);
    int ry = Radius2KernelSize (radius_y);
    int rz = Radius2KernelSize (radius_z);
    this->AddParameter ("scalar0", rx);
    this->AddParameter ("scalar1", ry);
    this->AddParameter ("scalar2", rz);
}

void
MeanSphereKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MeanSphereKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
