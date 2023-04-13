
#include "cleDetectMaximaKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMeanBoxKernel.hpp"
#include "cle_detect_maxima.h"


#include <algorithm>

namespace cle
{

DetectMaximaKernel::DetectMaximaKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("detect_maxima", oclKernel::detect_maxima);
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

} // namespace cle
