#ifndef __CORE_CLESPERANTO_HPP
#define __CORE_CLESPERANTO_HPP

#include "cleBackend.hpp"
#include "cleImage.hpp"
#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"

#include <iostream>
#include <limits>
#include <type_traits>

namespace cle
{

class Clesperanto
{
public:
  using ProcessorPointer = std::shared_ptr<Processor>;
  using FloatLimits = std::numeric_limits<float>;
  using ShapeArray = std::array<size_t, 3>;

private:
  ProcessorPointer device_;

public:
  Clesperanto();

  auto
  WaitForKernelToFinish(const bool & flag = true) -> void;

  auto
  SelectDevice(const std::string & name = "", const std::string & type = "all") -> void;

  auto
  Info() -> std::string;

  template <class T = float>
  [[nodiscard]] auto
  Create(const ShapeArray & shape = { 1, 1, 1 }, const MemoryType & memory_type = MemoryType::BUFFER) const -> Image;

  template <class T = float>
  [[nodiscard]] auto
  Push(const std::vector<T> & array = { 0 },
       const ShapeArray &     shape = { 1, 1, 1 },
       const MemoryType &     memory_type = MemoryType::BUFFER) const -> Image;

  template <class T = float>
  [[nodiscard]] auto
  Pull(const Image & object) const -> std::vector<T>;

  [[nodiscard]] auto
  GetDevice() const -> ProcessorPointer;

  auto
  Absolute(const Image & source, const Image & destination) -> void;

  auto
  AddImageAndScalar(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  AddImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  AddImagesWeighted(const Image & source1,
                    const Image & source2,
                    const Image & destination,
                    const float & weight1 = 1,
                    const float & weight2 = 1) -> void;

  auto
  BinaryAnd(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  BinaryEdgeDetection(const Image & source, const Image & destination) -> void;

  auto
  BinaryNot(const Image & source, const Image & destination) -> void;

  auto
  BinaryOr(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  BinarySubtract(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  BinaryXor(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  BlockEnumerate(const Image & source, const Image & sum, const Image & destination, const int & block_size = 1)
    -> void;
  //! only 1d and does not support image

  auto
  CloseIndexGapsInLabelMap(const Image & source, const Image & destination, const int & block_size = 4096) -> void;

  auto
  ConnectedComponentLabelingBox(const Image & source, const Image & destination) -> void;

  auto
  Convolve(const Image & source, const Image & convolve_kernel, const Image & destination) -> void;

  auto
  Copy(const Image & source, const Image & destination) -> void;

  auto
  Crop(const Image & source, const Image & destination, const int & x = 0, const int & y = 0, const int & z = 0)
    -> void;

  auto
  DetectMaximaBox(const Image & source, const Image & destination) -> void;

  auto
  DifferenceOfGaussian(const Image & source,
                       const Image & destination,
                       const float & sigma1_x = 0,
                       const float & sigma1_y = 0,
                       const float & sigma1_z = 0,
                       const float & sigma2_x = 0,
                       const float & sigma2_y = 0,
                       const float & sigma2_z = 0) -> void;

  auto
  DilateBox(const Image & source, const Image & destination) -> void;

  auto
  DilateLabels(const Image & source, const Image & destination, const int & radius = 1) -> void;

  auto
  DilateSphere(const Image & source, const Image & destination) -> void;

  auto
  DivideImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  DivideImageAndScalar(const Image & source, const Image & destination, const float & scalar = 1) -> void;

  auto
  Equal(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  EqualConstant(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  ErodeBox(const Image & source, const Image & destination) -> void;

  auto
  ErodeSphere(const Image & source, const Image & destination) -> void;

  auto
  ExtendLabelingViaVoronoi(const Image & source, const Image & destination) -> void;

  auto
  FlagExistingLabels(const Image & source, const Image & destination) -> void;

  auto
  GaussianBlur(const Image & source,
               const Image & destination,
               const float & sigma_x = 0,
               const float & sigma_y = 0,
               const float & sigma_z = 0) -> void;

  auto
  GradientX(const Image & source, const Image & destination) -> void;

  auto
  GradientY(const Image & source, const Image & destination) -> void;

  auto
  GradientZ(const Image & source, const Image & destination) -> void;

  auto
  Greater(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  GreaterConstant(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  GreaterOrEqual(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  GreaterOrEqualConstant(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  Histogram(const Image & source,
            const Image & destination,
            const int &   bins = 255,
            const float & min_intensity = FloatLimits::infinity(),
            const float & max_intensity = FloatLimits::infinity()) -> void;

  auto
  LaplaceBox(const Image & source, const Image & destination) -> void;

  auto
  Mask(const Image & source, const Image & t_mask, const Image & destination) -> void;

  auto
  MaskedVoronoiLabeling(const Image & source, const Image & t_mask, const Image & destination) -> void;

  auto
  MaximumBox(const Image & source,
             const Image & destination,
             const int &   radius_x = 0,
             const int &   radius_y = 0,
             const int &   radius_z = 0) -> void;

  auto
  MaximumImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  MaximumOfAllPixels(const Image & source, const Image & destination) -> void;

  auto
  MaximumSphere(const Image & source,
                const Image & destination,
                const float & radius_x = 0,
                const float & radius_y = 0,
                const float & radius_z = 0) -> void;

  auto
  MaximumXProjection(const Image & source, const Image & destination) -> void;

  auto
  MaximumYProjection(const Image & source, const Image & destination) -> void;

  auto
  MaximumZProjection(const Image & source, const Image & destination) -> void;

  auto
  MeanBox(const Image & source,
          const Image & destination,
          const int &   radius_x = 0,
          const int &   radius_y = 0,
          const int &   radius_z = 0) -> void;

  auto
  MeanSphere(const Image & source,
             const Image & destination,
             const float & radius_x = 0,
             const float & radius_y = 0,
             const float & radius_z = 0) -> void;

  auto
  MinimumImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  MinimumBox(const Image & source,
             const Image & destination,
             const int &   radius_x = 0,
             const int &   radius_y = 0,
             const int &   radius_z = 0) -> void;

  auto
  MinimumOfAllPixels(const Image & source, const Image & destination) -> void;

  auto
  MinimumSphere(const Image & source,
                const Image & destination,
                const float & radius_x = 0,
                const float & radius_y = 0,
                const float & radius_z = 0) -> void;

  auto
  MinimumXProjection(const Image & source, const Image & destination) -> void;

  auto
  MinimumYProjection(const Image & source, const Image & destination) -> void;

  auto
  MinimumZProjection(const Image & source, const Image & destination) -> void;

  auto
  MultiplyImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  MultiplyImageAndScalar(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  NonzeroMinimumBox(const Image & source, const Image & t_flag, const Image & destination) -> void;

  auto
  NotEqual(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  NotEqualConstant(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  OnlyzeroOverwriteMaximumBox(const Image & source, const Image & flag, const Image & destination) -> void;

  auto
  OnlyzeroOverwriteMaximumDiamond(const Image & source, const Image & flag, const Image & destination) -> void;

  auto
  Power(const Image & source, const Image & destination, const float & exponent = 0) -> void;

  auto
  PowerImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  ReplaceIntensities(const Image & source, const Image & intensity_map, const Image & destination) -> void;

  auto
  ReplaceIntensity(const Image & source,
                   const Image & destination,
                   const float & input_intensity,
                   const float & output_intensity) -> void;

  auto
  Set(const Image & source, const float & scalar = 0) -> void;

  auto
  SetColumn(const Image & source, const int & column_index, const float & scalar = 0) -> void;

  auto
  SetNonzeroPixelsToPixelindex(const Image & source, const Image & destination) -> void;

  auto
  Smaller(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  SmallerConstant(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  SmallerOrEqual(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  SmallerOrEqualConstant(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  Sobel(const Image & source, const Image & destination) -> void;

  auto
  SubtractImageFromScalar(const Image & source, const Image & destination, const float & scalar = 0) -> void;

  auto
  SubtractImages(const Image & source1, const Image & source2, const Image & destination) -> void;

  auto
  SumOfAllPixels(const Image & source, const Image & destination) -> void;

  auto
  SumReductionX(const Image & source, const Image & destination, const int & block_size) -> void;

  auto
  SumXProjection(const Image & source, const Image & destination) -> void;

  auto
  SumYProjection(const Image & source, const Image & destination) -> void;

  auto
  SumZProjection(const Image & source, const Image & destination) -> void;

  auto
  ThresholdOtsu(const Image & source, const Image & destination) -> void;

  auto
  TopHatBox(const Image & source,
            const Image & destination,
            const int &   radius_x = 0,
            const int &   radius_y = 0,
            const int &   radius_z = 0) -> void;

  auto
  VoronoiOtsuLabeling(const Image & source,
                      const Image & destination,
                      const float & sigma_spot = 0,
                      const float & sigma_outline = 0) -> void;
};

template <class T>
auto
Clesperanto::Create(const ShapeArray & shape, const MemoryType & memory_type) const -> Image
{
  DataType data_type = TypeToDataType<T>();
  return Memory::AllocateMemory(this->GetDevice(), shape, data_type, memory_type);
}

template <class T>
auto
Clesperanto::Push(const std::vector<T> & array, const ShapeArray & shape, const MemoryType & memory_type) const -> Image
{
  DataType data_type = TypeToDataType<T>();
  auto     image = Memory::AllocateMemory(this->GetDevice(), shape, data_type, memory_type);
  Memory::WriteObject(image, array.data(), array.size());
  return image;
}

template <class T>
auto
Clesperanto::Pull(const Image & image) const -> std::vector<T>
{
  std::vector<T> array(image.Shape()[0] * image.Shape()[1] * image.Shape()[2]);
  Memory::ReadObject<T>(image, array.data(), array.size());
  return array;
}

} // namespace cle

#endif // __CORE_CLESPERANTO_HPP
