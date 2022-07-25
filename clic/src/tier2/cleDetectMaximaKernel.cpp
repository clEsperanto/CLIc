
#include "cleDetectMaximaKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMeanBoxKernel.hpp"

#include "cleUtils.hpp"

namespace cle
{

DetectMaximaKernel::DetectMaximaKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header_ = {
#include "cle_detect_maxima.h"
    };
    this->SetSource ("cle_detect_maxima", cl_header_);
}

void
DetectMaximaKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
DetectMaximaKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
DetectMaximaKernel::SetRadius (const int &radius_x, const int &radius_y, const int &radius_z)
{
    this->radius_ = { radius_x, radius_y, radius_z };
}

void
DetectMaximaKernel::Execute ()
{
    // if (this->radius_[0] > 0 || this->radius_[1] > 0 || this->radius_[2] > 0)
    //     {
    //         auto src = this->GetParameter<Object> ("src");
    //         auto dst = this->GetParameter<Object> ("dst");
    //         MeanBoxKernel mean (this->m_gpu);
    //         mean.SetInput (*src);
    //         mean.SetOutput (*dst);
    //         mean.SetRadius (this->radius_[0], this->radius_[1], this->radius_[2]);
    //         mean.Execute ();
    //         CopyKernel copy (this->m_gpu);
    //         copy.SetInput (*dst);
    //         copy.SetOutput (*src);
    //         copy.Execute ();
    //     }
}
} // namespace cle
