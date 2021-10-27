
#ifndef __clesperanto_hpp
#define __clesperanto_hpp

#include "cleGPU.hpp"
#include "cleBuffer.hpp"
#include "cleImage.hpp"
#include "cleObject.hpp"

#include <type_traits>
#include <iostream>

namespace cle
{

class Clesperanto
{
private:
    std::shared_ptr<cle::GPU> m_gpu;

public:
    Clesperanto();
    ~Clesperanto() = default;

    template<class T =float>
    cle::Buffer Create(const std::array<int,3>& ={1,1,1}) const;
    template<class T =float>
    cle::Image CreateImage(const std::array<int,3>& ={1,1,1}) const;
    template<class T =float>
    cle::Buffer Push(std::vector<T>& ={0}, const std::array<int,3>& ={1,1,1}) const;
    template<class T =float>
    cle::Image PushImage(std::vector<T>& ={0}, const std::array<int,3>& ={1,1,1}) const;
    template<class T =float>
    std::vector<T> Pull(cle::Buffer&) const;
    template<class T =float>
    std::vector<T> PullImage(cle::Image&) const;

    std::shared_ptr<GPU> Ressources();

    void AddImageAndScalar(Object&, Object&, float=0);
    void AddImagesWeighted(Object&, Object&, Object&, float=1, float=1);
    void AddImages(Object&, Object&, Object&);
    void SubtractImages(Object&, Object&, Object&);
    void DilateSphere(Object&, Object&);
    void ErodeSphere(Object&, Object&);
    void Equal(Object&, Object&, Object&);
    void EqualConstant(Object&, Object&, float=0);
    void GaussianBlur(Buffer&, Buffer&, float=1, float=1, float=0);
    void MeanBox(Buffer&, Buffer&, float=1, float=1, float=0);
    void MaximumBox(Buffer&, Buffer&, float=1, float=1, float=0);
    void MinimumBox(Buffer&, Buffer&, float=1, float=1, float=0);
    void Greater(Object&, Object&, Object&);
    void GreaterOrEqual(Object&, Object&, Object&);
    void GreaterConstant(Object&, Object&, float=0);
    void GreaterOrEqualConstant(Object&, Object&, float=0);
    void MaximumZProjection(Object&, Object&);
    void MaximumYProjection(Object&, Object&);
    void MaximumXProjection(Object&, Object&);
    void MaximumOfAllPixels(Buffer&, Buffer&);
    void MinimumZProjection(Object&, Object&);
    void MinimumYProjection(Object&, Object&);
    void MinimumXProjection(Object&, Object&);
    void MinimumOfAllPixels(Buffer&, Buffer&);
    void MeanSphere(Object&, Object&, int=1, int=1, int=0);
    void NonzeroMinimumBox(Object&, Object&, Object&);
    void NotEqual(Object&, Object&, Object&);
    void NotEqualConstant(Object&, Object&, float=0);
    void Absolute(Object&, Object&);
    void Sobel(Object&, Object&);
    void Set(Object&, float=0);
    void SetNonzeroPixelsToPixelindex(Object&, Object&);
    void DifferenceOfGaussian(Buffer&, Buffer&, float=0, float=0, float=0, float=1, float=1, float=0);
    void Smaller(Object&, Object&, Object&);
    void SmallerOrEqual(Object&, Object&, Object&);
    void SmallerConstant(Object&, Object&, float=0);
    void SmallerOrEqualConstant(Object&, Object&, float=0);
    void Copy(Object&, Object&);
    void DetectMaximaBox(Object&, Object&);
    void SumZProjection(Object&, Object&);
    void SumYProjection(Object&, Object&);
    void SumXProjection(Object&, Object&);
    void SumOfAllPixels(Buffer&, Buffer&);
    void ConnectedComponentLabellingBox(Buffer&, Buffer&);
    void ReplaceIntensity(Object&, Object&, float=0, float=0);
    void ReplaceIntensities(Object&, Object&, Object&);
    void SetColumn(Object&, int=0, float=0);
    void SumReductionX(Object&, Object&, int=0);
    void BlockEnumerate(Buffer&, Buffer&, Buffer&, int=0);
    void FlagExistingLabels(Buffer&, Buffer&);
    void CloseIndexGapsInLabelMap(Buffer&, Buffer&, int=4096);
};


    template<class T>
    cle::Buffer Clesperanto::Create(const std::array<int,3>& t_shape) const
    {
        return this->m_gpu->CreateBuffer<T>(t_shape);
    }

    template<class T>
    cle::Image Clesperanto::CreateImage(const std::array<int,3>& t_shape) const
    {
        return this->m_gpu->CreateImage<T>(t_shape);
    }

    template<class T>
    cle::Buffer Clesperanto::Push(std::vector<T>& t_array, const std::array<int,3>& t_shape) const
    {
        return this->m_gpu->PushBuffer<T>(t_array, t_shape);
    }

    template<class T>
    cle::Image Clesperanto::PushImage(std::vector<T>& t_array, const std::array<int,3>& t_shape) const
    {
        return this->m_gpu->PushImage<T>(t_array, t_shape);
    }

    template<class T>
    std::vector<T> Clesperanto::Pull(cle::Buffer& t_data) const
    {
        return this->m_gpu->Pull<T>(t_data);
    }

    template<class T>
    std::vector<T> Clesperanto::PullImage(cle::Image& t_data) const
    {
        return this->m_gpu->Pull<T>(t_data);
    }


} // namespace cle

#endif //__clesperanto_hpp