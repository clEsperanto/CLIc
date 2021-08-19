
#ifndef __clesperanto_hpp
#define __clesperanto_hpp

#include "cleGPU.hpp"
#include "cleDataType.hpp"
#include "cleOperations.hpp"

#include <type_traits>
#include <iostream>

namespace cle
{

class Clesperanto
{
private:
    std::shared_ptr<GPU> m_gpu;

    template<class T>
    Buffer::DataType Template2DataType();

public:
    Clesperanto();
    ~Clesperanto() = default;

    template<class T>
    Buffer Push(std::vector<T>& arr, int[3]);

    template<class T>
    std::vector<T> Pull(Buffer& gpu_obj);

    template<class T>
    Buffer Create(std::vector<T>& arr, int[3]);

    template<class T>
    Buffer Create(Buffer& gpu_obj);

    template<class T>
    Buffer Create(int[3]);

    template<class T>
    Buffer Create();

    std::shared_ptr<GPU> GetGPU();

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
    Buffer::DataType Clesperanto::Template2DataType()
    {
        if (std::is_same<T, float>::value)  return Buffer::FLOAT;
        if(std::is_same<T, int>::value) return Buffer::INT;
        if(std::is_same<T, unsigned int>::value) return Buffer::UINT;
        if(std::is_same<T, char>::value) return Buffer::CHAR;
        if(std::is_same<T, unsigned char>::value) return Buffer::UCHAR;
        if(std::is_same<T, double>::value) return Buffer::DOUBLE;
        if(std::is_same<T, short>::value) return Buffer::SHORT;
        if(std::is_same<T, unsigned short>::value) return Buffer::USHORT;
        return Buffer::FLOAT;
    }


    template<class T>
    Buffer Clesperanto::Push(std::vector<T>& arr, int dimensions[3])
    {
        cl::Buffer obj = CreateBuffer<T>(arr.size(), this->m_gpu);
        WriteBuffer<T>(obj, arr.data(), arr.size(), this->m_gpu);
        Buffer::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dimensions, dataType);
    }

    template<class T>
    std::vector<T> Clesperanto::Pull(Buffer& obj)
    {
        std::vector<T> arr(obj.GetSize());
        ReadBuffer<T>(obj.GetObject(), arr.data(), obj.GetSize(), this->m_gpu);
        return arr;
    }

    template<class T>
    Buffer Clesperanto::Create(std::vector<T>& arr, int dimensions[3])
    {
        cl::Buffer obj = CreateBuffer<T>(arr.size(), this->m_gpu);
        Buffer::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dimensions, dataType);
    }

    template<class T>
    Buffer Clesperanto::Create(Buffer& obj)
    {
        cl::Buffer new_obj = CreateBuffer<T>(obj.GetSize(), this->m_gpu);
        Buffer::DataType dataType = this->Template2DataType<T>();
        return Buffer (new_obj, obj.GetShape(), dataType);
       
    }

    template<class T>
    Buffer Clesperanto::Create(int dimensions[3])
    {
        size_t size = dimensions[0]*dimensions[1]*dimensions[2];
        cl::Buffer obj = CreateBuffer<T>(size, this->m_gpu);
        Buffer::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dimensions, dataType);
    }

    template<class T>
    Buffer Clesperanto::Create()
    {
        cl::Buffer obj = CreateBuffer<T>(1, this->m_gpu);
        Buffer::DataType dataType = this->Template2DataType<T>();
        return Buffer (obj, dataType);
    }

} // namespace cle

#endif //__clesperanto_hpp