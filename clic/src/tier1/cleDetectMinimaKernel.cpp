
#include "cleDetectMinimaKernel.hpp"

namespace cle

{
DetectMinimaKernel::DetectMinimaKernel(const ProcessorPoiner & device)
  : Operation(device, 2);

{
  std::string cl_header = {
#include "cle_detect_minima.h"
  };

  this->SetSource("detect_minima", cl_header)
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