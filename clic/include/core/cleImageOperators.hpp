#ifndef __CORE_CLEIMAGEOPERATORS_HPP
#define __CORE_CLEIMAGEOPERATORS_HPP


#include "cleImage.hpp"
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"

#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleMinimumXProjectionKernel.hpp"
#include "cleMinimumYProjectionKernel.hpp"
#include "cleMinimumZProjectionKernel.hpp"

#include "cleSumOfAllPixelsKernel.hpp"
#include "cleSumXProjectionKernel.hpp"
#include "cleSumYProjectionKernel.hpp"
#include "cleSumZProjectionKernel.hpp"

#include "cleMemory.hpp"

namespace cle
{

class ImageOperators
{
private:
  /* data */
public:
  ImageOperators(/* args */);
  ~ImageOperators();

  [[nodiscard]] auto
  Maximum(const Image & image, const int & axis = -1) -> Image;
};

ImageOperators::ImageOperators(/* args */) {}

ImageOperators::~ImageOperators() {}


auto
Maximum(const Image & image, const int & axis = -1) -> Image
{
  Image::ShapeArray          new_shape = image.Shape();
  std::unique_ptr<Operation> kernel;
  switch (axis)
  {
    case 0:
      new_shape[0] = 1;
      kernel = std::unique_ptr<Operation>(new MaximumXProjectionKernel(image.Device()));
      break;
    case 1:
      new_shape[1] = 1;
      kernel = std::unique_ptr<Operation>(new MaximumYProjectionKernel(image.Device()));
      break;
    case 2:
      new_shape[2] = 1;
      kernel = std::unique_ptr<Operation>(new MaximumZProjectionKernel(image.Device()));
      break;
    default:
      new_shape = { 1, 1, 1 };
      kernel = std::unique_ptr<Operation>(new MaximumOfAllPixelsKernel(image.Device()));
      break;
  }
  auto output = Memory::AllocateMemory(image.Device(), new_shape, image.GetDataType(), image.GetMemoryType());
  kernel->AddParameter("src", image);
  kernel->AddParameter("dst", output);
  kernel->Execute();
  return output;
}

auto
Minimum(const Image & image, const int & axis = -1) -> Image
{
  Image::ShapeArray          new_shape = image.Shape();
  std::unique_ptr<Operation> kernel;
  switch (axis)
  {
    case 0:
      new_shape[0] = 1;
      kernel = std::unique_ptr<Operation>(new MinimumXProjectionKernel(image.Device()));
      break;
    case 1:
      new_shape[1] = 1;
      kernel = std::unique_ptr<Operation>(new MinimumYProjectionKernel(image.Device()));
      break;
    case 2:
      new_shape[2] = 1;
      kernel = std::unique_ptr<Operation>(new MinimumZProjectionKernel(image.Device()));
      break;
    default:
      new_shape = { 1, 1, 1 };
      kernel = std::unique_ptr<Operation>(new MinimumOfAllPixelsKernel(image.Device()));
      break;
  }
  auto output = Memory::AllocateMemory(image.Device(), new_shape, image.GetDataType(), image.GetMemoryType());
  kernel->AddParameter("src", image);
  kernel->AddParameter("dst", output);
  kernel->Execute();
  return output;
}

auto
Sum(const Image & image, const int & axis = -1) -> Image
{
  Image::ShapeArray          new_shape = image.Shape();
  std::unique_ptr<Operation> kernel;
  switch (axis)
  {
    case 0:
      new_shape[0] = 1;
      kernel = std::unique_ptr<Operation>(new SumXProjectionKernel(image.Device()));
      break;
    case 1:
      new_shape[1] = 1;
      kernel = std::unique_ptr<Operation>(new SumYProjectionKernel(image.Device()));
      break;
    case 2:
      new_shape[2] = 1;
      kernel = std::unique_ptr<Operation>(new SumZProjectionKernel(image.Device()));
      break;
    default:
      new_shape = { 1, 1, 1 };
      kernel = std::unique_ptr<Operation>(new SumOfAllPixelsKernel(image.Device()));
      break;
  }
  auto output = Memory::AllocateMemory(image.Device(), new_shape, image.GetDataType(), image.GetMemoryType());
  kernel->AddParameter("src", image);
  kernel->AddParameter("dst", output);
  kernel->Execute();
  return output;
}

} // namespace cle


#endif // __CORE_CLEIMAGEOPERATORS_HPP
