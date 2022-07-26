#ifndef __CORE_CLESPERANTO_HPP
#define __CORE_CLESPERANTO_HPP

#include "cleBackend.hpp"
#include "cleImage.hpp"
#include "cleMemory.hpp"
#include "cleProcessor.hpp"

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
    using MemType = cl_mem_object_type;

  private:
    ProcessorPointer device_;

  public:
    Clesperanto ();

    template <class T = float>
    [[nodiscard]] auto Create (const ShapeArray &shape = { 1, 1, 1 }, const MemType &type = CL_MEM_OBJECT_BUFFER) const -> Image;
    template <class T = float>
    auto Push (const std::vector<T> &array = { 0 }, const ShapeArray &shape = { 1, 1, 1 }, const MemType &type = CL_MEM_OBJECT_BUFFER) const -> Image;
    template <class T = float>
    auto Pull (const Image &object) const -> std::vector<T>;

    [[nodiscard]] auto GetDevice () const -> ProcessorPointer;

    auto Absolute (const Image &, const Image &) -> void;
    auto AddImageAndScalar (const Image &, const Image &, const float & = 0) -> void;
    auto AddImagesWeighted (const Image &, const Image &, const Image &, const float & = 1, const float & = 1) -> void;
    auto AddImages (const Image &, const Image &, const Image &) -> void;

    auto BinaryAnd (const Image &, const Image &, const Image &) -> void;
    auto BinaryOr (const Image &, const Image &, const Image &) -> void;
    auto BinaryNot (const Image &, const Image &) -> void;
    auto BinarySubtract (const Image &, const Image &, const Image &) -> void;
    auto BinaryXor (const Image &, const Image &, const Image &) -> void;
    auto BlockEnumerate (const Image &, const Image &, const Image &, const int & = 0) -> void; //! block enumarate fail when running with Image

    auto Copy (const Image &, const Image &) -> void;
    auto ConnectedComponentLabelingBox (const Image &, const Image &) -> void;
    auto CloseIndexGapsInLabelMap (const Image &, const Image &, const int & = 4096) -> void;

    auto DilateSphere (const Image &, const Image &) -> void;
    auto DifferenceOfGaussian (const Image &, const Image &, const float & = 0, const float & = 0, const float & = 0, const float & = 1, const float & = 1, const float & = 0) -> void;
    auto DetectMaximaBox (const Image &, const Image &, const int & = 0, const int & = 0, const int & = 0) -> void;
    auto DivideImages (const Image &, const Image &, const Image &) -> void;
    auto DilateLabels (const Image &, const Image &, const int & = 1) -> void;

    auto ErodeSphere (const Image &, const Image &) -> void;
    auto Equal (const Image &, const Image &, const Image &) -> void;
    auto EqualConstant (const Image &, const Image &, const float & = 0) -> void;
    auto ExtendLabelingViaVoronoi (const Image &, const Image &) -> void;

    auto FlagExistingLabels (const Image &, const Image &) -> void;

    auto GaussianBlur (const Image &, const Image &, const float & = 1, const float & = 1, const float & = 0) -> void;
    auto Greater (const Image &, const Image &, const Image &) -> void;
    auto GreaterOrEqual (const Image &, const Image &, const Image &) -> void;
    auto GreaterConstant (const Image &, const Image &, const float & = 0) -> void;
    auto GreaterOrEqualConstant (const Image &, const Image &, const float & = 0) -> void;

    auto Histogram (const Image &, const Image &, const int & = 256, const float & = FloatLimits::infinity (), const float & = FloatLimits::infinity ()) -> void;

    auto MeanBox (const Image &, const Image &, const int & = 1, const int & = 1, const int & = 1) -> void;
    auto MaximumBox (const Image &, const Image &, const int & = 1, const int & = 1, const int & = 1) -> void;
    auto MinimumBox (const Image &, const Image &, const int & = 1, const int & = 1, const int & = 1) -> void;
    auto Mask (const Image &, const Image &, const Image &) -> void;
    auto MaskedVoronoiLabeling (const Image &, const Image &, const Image &) -> void;
    auto MaximumZProjection (const Image &, const Image &) -> void;
    auto MaximumYProjection (const Image &, const Image &) -> void;
    auto MaximumXProjection (const Image &, const Image &) -> void;
    auto MaximumOfAllPixels (const Image &, const Image &) -> void;
    auto MinimumZProjection (const Image &, const Image &) -> void;
    auto MinimumYProjection (const Image &, const Image &) -> void;
    auto MinimumXProjection (const Image &, const Image &) -> void;
    auto MinimumOfAllPixels (const Image &, const Image &) -> void;
    auto MeanSphere (const Image &, const Image &, const int & = 1, const int & = 1, const int & = 0) -> void;
    auto MultiplyImages (const Image &, const Image &, const Image &) -> void;

    auto NonzeroMinimumBox (const Image &, const Image &, const Image &) -> void;
    auto NotEqual (const Image &, const Image &, const Image &) -> void;
    auto NotEqualConstant (const Image &, const Image &, const float & = 0) -> void;

    auto OnlyzeroOverwriteMaximumBox (const Image &, const Image &, const Image &) -> void;
    auto OnlyzeroOverwriteMaximumDiamond (const Image &, const Image &, const Image &) -> void;

    auto ReplaceIntensity (const Image &, const Image &, const float & = 0, const float & = 0) -> void;
    auto ReplaceIntensities (const Image &, const Image &, const Image &) -> void;

    auto Sobel (const Image &, const Image &) -> void;
    auto SubtractImages (const Image &, const Image &, const Image &) -> void;
    auto Set (const Image &, const float & = 0) -> void;
    auto SetNonzeroPixelsToPixelindex (const Image &, const Image &) -> void;
    auto Smaller (const Image &, const Image &, const Image &) -> void;
    auto SmallerOrEqual (const Image &, const Image &, const Image &) -> void;
    auto SmallerConstant (const Image &, const Image &, const float & = 0) -> void;
    auto SmallerOrEqualConstant (const Image &, const Image &, const float & = 0) -> void;
    auto SumZProjection (const Image &, const Image &) -> void;
    auto SumYProjection (const Image &, const Image &) -> void;
    auto SumXProjection (const Image &, const Image &) -> void;
    auto SumOfAllPixels (const Image &, const Image &) -> void;
    auto SetColumn (const Image &, const int & = 0, const float & = 0) -> void;
    auto SumReductionX (const Image &, const Image &, const int & = 0) -> void;
    auto SubtractImageFromScalar (const Image &, const Image &, const float & = 0) -> void;

    auto ThresholdOtsu (const Image &, const Image &) -> void;
    auto TopHatBox (const Image &, const Image &, const int & = 1, const int & = 1, const int & = 1) -> void;

    auto VoronoiOtsuLabeling (const Image &, const Image &, const float & = 1, const float & = 1) -> void;
};

template <class T>
auto
Clesperanto::Create (const ShapeArray &shape, const MemType &type) const -> Image
{
    DataType data_type{};
    data_type.Set<T> ();
    return Memory::AllocateObject (this->GetDevice (), shape, data_type.Get (), type);
}

template <class T>
auto
Clesperanto::Push (const std::vector<T> &array, const ShapeArray &shape, const MemType &type) const -> Image
{
    DataType data_type{};
    data_type.Set<T> ();
    auto image = Memory::AllocateObject (this->GetDevice (), shape, data_type.Get (), type);
    Memory::WriteObject (image, array);
    return image;
}

template <class T>
auto
Clesperanto::Pull (const Image &object) const -> std::vector<T>
{
    return Memory::ReadObject<T> (object);
}

} // namespace cle

#endif // __CORE_CLESPERANTO_HPP
