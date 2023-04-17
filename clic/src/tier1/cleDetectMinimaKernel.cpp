
#include "cleDetectMinimaKernel.hpp"
#include "cle_detect_minima.h"


namespace cle
{

DetectMinimaKernel::DetectMinimaKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("detect_minima", oclKernel::detect_minima);
}

auto
DetectMinimaKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
DetectMinimaKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}
} // namespace cle