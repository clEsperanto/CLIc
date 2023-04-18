#include "cleBinaryEdgeDetectionKernel.hpp"
#include "cle_binary_edge_detection.h"

namespace cle
{

BinaryEdgeDetectionKernel::BinaryEdgeDetectionKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  this->SetSource("binary_edge_detection", oclKernel::binary_edge_detection);
}

void
BinaryEdgeDetectionKernel::SetInput(const Image & object)
{
  this->AddParameter("src", object);
}

void
BinaryEdgeDetectionKernel::SetOutput(const Image & object)
{
  this->AddParameter("dst", object);
}

} // namespace cle