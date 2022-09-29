
#include "cleHistogramKernel.hpp"

#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMemory.hpp"
#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleSumZProjectionKernel.hpp"


namespace cle
{

HistogramKernel::HistogramKernel(const ProcessorPointer & device)
  : Operation(device, 8, 1)
{
  std::string cl_header_ = {
#include "cle_histogram.h"
  };
  this->SetSource("histogram", cl_header_);
}

auto
HistogramKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
HistogramKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("histogram", object);
}

auto
HistogramKernel::SetMinimumIntensity(const float & value) -> void
{
  this->min_intensity_ = value;
}

auto
HistogramKernel::SetMaximumIntensity(const float & value) -> void
{
  this->max_intensity_ = value;
}

auto
HistogramKernel::SetSteps(const int & step_x, const int & step_y, const int & step_z) -> void
{
  this->AddParameter("step_size_x", step_x);
  this->AddParameter("step_size_y", step_y);
  this->AddParameter("step_size_z", step_z);
}

auto
HistogramKernel::SetNumBins(const size_t & bin) -> void
{
  this->nb_bins_ = bin;
  this->AddConstant("NUMBER_OF_HISTOGRAM_BINS", this->nb_bins_);
}

auto
HistogramKernel::Execute() -> void
{
  float        infinity = std::numeric_limits<float>::infinity();
  auto         dst = this->GetImage("histogram");
  auto         src = this->GetImage("src");
  const size_t nb_partial_hist = src->Shape()[1];

  if (this->min_intensity_ == infinity || this->max_intensity_ == infinity)
  {
    auto temp_scalar_buffer = Memory::AllocateMemory(this->GetDevice(), { 1, 1, 1 });

    MinimumOfAllPixelsKernel minimum_intensity_kernel(this->GetDevice());
    minimum_intensity_kernel.SetInput(*src);
    minimum_intensity_kernel.SetOutput(temp_scalar_buffer);
    minimum_intensity_kernel.Execute();
    this->min_intensity_ = Memory::ReadObject<float>(temp_scalar_buffer).front();

    MaximumOfAllPixelsKernel maximum_intensity_kernel(this->GetDevice());
    maximum_intensity_kernel.SetInput(*src);
    maximum_intensity_kernel.SetOutput(temp_scalar_buffer);
    maximum_intensity_kernel.Execute();
    this->max_intensity_ = Memory::ReadObject<float>(temp_scalar_buffer).front();
  }
  this->AddParameter("minimum", this->min_intensity_);
  this->AddParameter("maximum", this->max_intensity_);

  auto partial_hist = Memory::AllocateMemory(this->GetDevice(), { this->nb_bins_, 1, nb_partial_hist });
  this->AddParameter("dst", partial_hist);
  this->SetRange(std::array<size_t, 3>{ nb_partial_hist, 1, 1 });
  this->Operation::Execute();

  SumZProjectionKernel sum(this->GetDevice());
  sum.SetInput(partial_hist);
  sum.SetOutput(*dst);
  sum.Execute();
}

} // namespace cle