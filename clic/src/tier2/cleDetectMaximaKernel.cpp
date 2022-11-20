
#include "cleDetectMaximaKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMeanBoxKernel.hpp"


#include <algorithm>

namespace cle
{

DetectMaximaKernel::DetectMaximaKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header_ = {
#include "cle_detect_maxima.h"
  };
  this->SetSource("detect_maxima", cl_header_);
}

auto
DetectMaximaKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
DetectMaximaKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

// remove radius
// auto
// DetectMaximaKernel::SetRadius (const int &radius_x, const int &radius_y, const int &radius_z) -> void
// {
//     this->radius_ = { radius_x, radius_y, radius_z };
// }
// remove radius
// auto
// DetectMaximaKernel::Execute () -> void
// {
//     if (std::any_of (radius_.begin (), radius_.end (), [] (int i) { return i > 0; }))
//         {
//             auto src = this->GetImage ("src");
//             auto dst = this->GetImage ("dst");

//             MeanBoxKernel mean (this->Device ());
//             mean.SetInput (*src);
//             mean.SetOutput (*dst);
//             mean.SetRadius (this->radius_[0], this->radius_[1], this->radius_[2]);
//             mean.Execute ();

//             CopyKernel copy (this->Device ());
//             copy.SetInput (*dst);
//             copy.SetOutput (*src);
//             copy.Execute ();
//         }

//     this->Operation::Execute ();
// }
} // namespace cle
