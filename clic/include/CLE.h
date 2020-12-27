/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __CLE_h
#define __CLE_h



#include "cleGPU.h"
#include "cleObject.h"
#include "image.h"
#include "cleBuffer.h"

#include "utils.h"


namespace cle
{

class CLE
{
private:
    GPU gpu;

public:
    CLE(GPU&);
    ~CLE(){};

    template<class T>
    Buffer Push(Image<T>& image);
    template<class T>
    Image<T> Pull(Buffer& gpu_obj);
    template<class T>
    Buffer Create(Image<T>& image, std::string type = "");
    template<class T>
    Buffer Create(Buffer& gpu_obj, std::string type = "");
    template<class T>
    Buffer Create(unsigned int dimensions[3], std::string type);

    GPU GetGPU();

    void AddImageAndScalar(Buffer&, Buffer&, float);
    void AddImagesWeighted(Buffer&, Buffer&, Buffer&, float=1, float=1);
    void AddImages(Buffer&, Buffer&, Buffer&);
    void SubtractImages(Buffer&, Buffer&, Buffer&);
    void DilateSphere(Buffer&, Buffer&);
    void ErodeSphere(Buffer&, Buffer&);
    void Equal(Buffer&, Buffer&, Buffer&);
    void EqualConstant(Buffer&, Buffer&, float);
    void GaussianBlur3D(Buffer&, Buffer&, float=0, float=0, float=0);
    void GaussianBlur2D(Buffer&, Buffer&, float=0, float=0);
    void Maximum2DBox(Buffer&, Buffer&, float=0, float=0);
    void Minimum2DBox(Buffer&, Buffer&, float=0, float=0);
    void Mean2DBox(Buffer&, Buffer&, float=0, float=0);
    void Maximum3DBox(Buffer&, Buffer&, float=0, float=0, float=0);
    void Minimum3DBox(Buffer&, Buffer&, float=0, float=0, float=0);
    void Mean3DBox(Buffer&, Buffer&, float=0, float=0, float=0);
    void Greater(Buffer&, Buffer&, Buffer&);
    void GreaterOrEqual(Buffer&, Buffer&, Buffer&);
    void GreaterConstant(Buffer&, Buffer&, float);
    void GreaterOrEqualConstant(Buffer&, Buffer&, float);
    void MaximumZProjection(Buffer&, Buffer&);
    void MaximumYProjection(Buffer&, Buffer&);
    void MaximumXProjection(Buffer&, Buffer&);
    void MaximumOfAllPixels(Buffer&, Buffer&);
    void MinimumZProjection(Buffer&, Buffer&);
    void MinimumYProjection(Buffer&, Buffer&);
    void MinimumXProjection(Buffer&, Buffer&);
    void MinimumOfAllPixels(Buffer&, Buffer&);
    void Mean2DSphere(Buffer&, Buffer&, int=1, int=1);
    void NonzeroMinimumBox(Buffer&, Buffer&, Buffer&);
    void NotEqual(Buffer&, Buffer&, Buffer&);
    void NotEqualConstant(Buffer&, Buffer&, float);
    void Absolute(Buffer&, Buffer&);
    void Sobel(Buffer&, Buffer&);
    void Set(Buffer&, float);
    void SetNonzeroPixelsToPixelindex(Buffer&, Buffer&);
    void DifferenceOfGaussian2D(Buffer&, Buffer&, float=2, float=2, float=2, float=2);
    void DifferenceOfGaussian3D(Buffer&, Buffer&, float=2, float=2, float=2, float=2, float=2, float=2);
    void Smaller(Buffer&, Buffer&, Buffer&);
    void SmallerOrEqual(Buffer&, Buffer&, Buffer&);
    void SmallerConstant(Buffer&, Buffer&, float);
    void SmallerOrEqualConstant(Buffer&, Buffer&, float);
    void Copy(Buffer&, Buffer&);
    void DetectMaximaBox(Buffer&, Buffer&);
    void SumZProjection(Buffer&, Buffer&);
    void SumYProjection(Buffer&, Buffer&);
    void SumXProjection(Buffer&, Buffer&);
    void SumOfAllPixels(Buffer&, Buffer&);
    void ConnectedComponentsLabelingBox(Buffer&, Buffer&);
    void ReplaceIntensity(Buffer&, Buffer&, float, float);
    void SetColumn(Buffer&, int, float);
    void SumReduction(Buffer&, Buffer&, int);

};


template<class T>
Buffer CLE::Push(Image<T>& image)
{
    cl_mem mem_obj = CreateBuffer<T>(image.GetDataSize(), this->gpu);
    bool res = WriteBuffer<T>(mem_obj, image.GetData(), image.GetDataSize(), this->gpu);
    return Buffer (mem_obj, image.GetDimensions(), image.GetType());
}

template<class T>
Image<T> CLE::Pull(Buffer& gpu_obj)
{
    unsigned int arrSize = gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    T* output_arr = ReadBuffer<T>(gpu_obj.GetData(), sizeof(T) * arrSize, this->gpu);
    Image<T> image (output_arr, gpu_obj.GetDimensions()[0], gpu_obj.GetDimensions()[1], gpu_obj.GetDimensions()[2], gpu_obj.GetDataType());
    return image;        
}

template<class T>
Buffer CLE::Create(Image<T>& image, std::string type)
{
    cl_mem mem_obj = CreateBuffer<T>(image.GetNbPixels(), this->gpu);
    if (type.empty())
    {
        type = image.GetType();
    }
    return Buffer (mem_obj, image.GetDimensions(), type);
}

template<class T>
Buffer CLE::Create(Buffer& gpu_obj, std::string type)
{
    size_t arrSize = gpu_obj.GetDimensions()[0] * gpu_obj.GetDimensions()[1] * gpu_obj.GetDimensions()[2];
    cl_mem mem_obj = CreateBuffer<T>(arrSize, this->gpu);
    if (type.empty())
    {
        type = gpu_obj.GetDataType();
    }
    return Buffer (mem_obj, gpu_obj.GetDimensions(), type);
}

template<class T>
Buffer CLE::Create(unsigned int dimensions[3], std::string type)
{
    size_t arrSize = dimensions[0] * dimensions[1] * dimensions[2];
    cl_mem mem_obj = CreateBuffer<T>(arrSize, this->gpu);
    return Buffer (mem_obj, dimensions, type);
}

} // namespace cle

#endif //__CLE_h