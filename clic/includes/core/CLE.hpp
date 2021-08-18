
#ifndef __CLE_hpp
#define __CLE_hpp

#include "cleGPU.hpp"
#include "cleBuffer.hpp"
#include "cleOperations.hpp"

#include <type_traits>
#include <iostream>

namespace cle
{

class CLE
{
private:
    GPU m_gpu;

    template<class T>
    LightObject::DataType Template2DataType();

public:
    CLE(GPU&);
    ~CLE() = default;

    template<class T>
    Buffer Push(std::vector<T>& arr, unsigned int[3]);

    template<class T>
    std::vector<T> Pull(Buffer& gpu_obj);

    template<class T>
    Buffer Create(std::vector<T>& arr, unsigned int[3]);

    template<class T>
    Buffer Create(Buffer& gpu_obj);

    template<class T>
    Buffer Create(unsigned int[3]);

    template<class T>
    Buffer Create();

    GPU GetGPU();

    void AddImageAndScalar(Buffer&, Buffer&, float=0);
    void AddImagesWeighted(Buffer&, Buffer&, Buffer&, float=1, float=1);
    void AddImages(Buffer&, Buffer&, Buffer&);
    void SubtractImages(Buffer&, Buffer&, Buffer&);
    void DilateSphere(Buffer&, Buffer&);
    void ErodeSphere(Buffer&, Buffer&);
    void Equal(Buffer&, Buffer&, Buffer&);
    void EqualConstant(Buffer&, Buffer&, float=0);
    void GaussianBlur(Buffer&, Buffer&, float=1, float=1, float=0);
    void MeanBox(Buffer&, Buffer&, float=1, float=1, float=0);
    void MaximumBox(Buffer&, Buffer&, float=1, float=1, float=0);
    void MinimumBox(Buffer&, Buffer&, float=1, float=1, float=0);
    void Greater(Buffer&, Buffer&, Buffer&);
    void GreaterOrEqual(Buffer&, Buffer&, Buffer&);
    void GreaterConstant(Buffer&, Buffer&, float=0);
    void GreaterOrEqualConstant(Buffer&, Buffer&, float=0);
    void MaximumZProjection(Buffer&, Buffer&);
    void MaximumYProjection(Buffer&, Buffer&);
    void MaximumXProjection(Buffer&, Buffer&);
    void MaximumOfAllPixels(Buffer&, Buffer&);
    void MinimumZProjection(Buffer&, Buffer&);
    void MinimumYProjection(Buffer&, Buffer&);
    void MinimumXProjection(Buffer&, Buffer&);
    void MinimumOfAllPixels(Buffer&, Buffer&);
    void MeanSphere(Buffer&, Buffer&, int=1, int=1, int=0);
    void NonzeroMinimumBox(Buffer&, Buffer&, Buffer&);
    void NotEqual(Buffer&, Buffer&, Buffer&);
    void NotEqualConstant(Buffer&, Buffer&, float=0);
    void Absolute(Buffer&, Buffer&);
    void Sobel(Buffer&, Buffer&);
    void Set(Buffer&, float=0);
    void SetNonzeroPixelsToPixelindex(Buffer&, Buffer&);
    void DifferenceOfGaussian(Buffer&, Buffer&, float=0, float=0, float=0, float=1, float=1, float=0);
    void Smaller(Buffer&, Buffer&, Buffer&);
    void SmallerOrEqual(Buffer&, Buffer&, Buffer&);
    void SmallerConstant(Buffer&, Buffer&, float=0);
    void SmallerOrEqualConstant(Buffer&, Buffer&, float=0);
    void Copy(Buffer&, Buffer&);
    void DetectMaximaBox(Buffer&, Buffer&);
    void SumZProjection(Buffer&, Buffer&);
    void SumYProjection(Buffer&, Buffer&);
    void SumXProjection(Buffer&, Buffer&);
    void SumOfAllPixels(Buffer&, Buffer&);
    void ConnectedComponentLabellingBox(Buffer&, Buffer&);
    void ReplaceIntensity(Buffer&, Buffer&, float=0, float=0);
    void ReplaceIntensities(Buffer&, Buffer&, Buffer&);
    void SetColumn(Buffer&, int=0, float=0);
    void SumReductionX(Buffer&, Buffer&, int=0);
    void BlockEnumerate(Buffer&, Buffer&, Buffer&, int=0);
    void FlagExistingLabels(Buffer&, Buffer&);
};

    template<class T>
    LightObject::DataType CLE::Template2DataType()
    {
        if (std::is_same<T, float>::value) 
        {
            return LightObject::Float;
        }
        else if(std::is_same<T, int>::value)
        {
            return LightObject::Int;
        }
        else if(std::is_same<T, unsigned int>::value)
        {
            return LightObject::UInt;
        }
        else if(std::is_same<T, char>::value)
        {
            return LightObject::Char;
        }
        else if(std::is_same<T, unsigned char>::value)
        {
            return LightObject::UChar;
        }
        else if(std::is_same<T, short>::value)
        {
            return LightObject::Short;
        }
        else if(std::is_same<T, unsigned short>::value)
        {
            return LightObject::UShort;
        }
        else  // default
        {
            return LightObject::Float;
        }
    }


    template<class T>
    Buffer CLE::Push(std::vector<T>& arr, unsigned int dimensions[3])
    {
        cl::Buffer obj = CreateBuffer<T>(arr.size(), this->m_gpu);
        WriteBuffer<T>(obj, arr.data(), arr.size(), this->m_gpu);
        LightObject::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dimensions, dataType);
    }

    template<class T>
    std::vector<T> CLE::Pull(Buffer& obj)
    {
        std::vector<T> arr(obj.GetSize());
        ReadBuffer<T>(obj.GetObject(), arr.data(), obj.GetSize(), this->m_gpu);
        return arr;
    }

    template<class T>
    Buffer CLE::Create(std::vector<T>& arr, unsigned int dimensions[3])
    {
        cl::Buffer obj = CreateBuffer<T>(arr.size(), this->m_gpu);
        LightObject::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dimensions, dataType);
    }

    template<class T>
    Buffer CLE::Create(Buffer& obj)
    {
        cl::Buffer new_obj = CreateBuffer<T>(obj.GetSize(), this->m_gpu);
        LightObject::DataType dataType = this->Template2DataType<T>();
        return Buffer (new_obj, obj.GetDimensions(), dataType);
       
    }

    template<class T>
    Buffer CLE::Create(unsigned int dimensions[3])
    {
        size_t size = dimensions[0]*dimensions[1]*dimensions[2];
        cl::Buffer obj = CreateBuffer<T>(size, this->m_gpu);
        LightObject::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dimensions, dataType);
    }

    template<class T>
    Buffer CLE::Create()
    {
        cl::Buffer obj = CreateBuffer<T>(1, this->m_gpu);
        LightObject::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dataType);
    }

} // namespace cle

#endif //__CLE_hpp