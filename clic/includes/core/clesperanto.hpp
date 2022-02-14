
#ifndef __clesperanto_hpp
#define __clesperanto_hpp

#include "cleGPU.hpp"
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
    const cle::Object Create(const std::array<size_t,3>& ={1,1,1}, const std::string& ="buffer") const;
    template<class T =float>
    const cle::Object Push(const std::vector<T>& ={0}, const std::array<size_t,3>& ={1,1,1}, const std::string& ="buffer") const;
    template<class T =float>
    const std::vector<T> Pull(const cle::Object&) const;

    std::shared_ptr<GPU> Ressources();

    void AddImageAndScalar(Object&, Object&, float=0);
    void AddImagesWeighted(Object&, Object&, Object&, float=1, float=1);
    void AddImages(Object&, Object&, Object&);
    void SubtractImages(Object&, Object&, Object&);
    void DilateSphere(Object&, Object&);
    void ErodeSphere(Object&, Object&);
    void Equal(Object&, Object&, Object&);
    void EqualConstant(Object&, Object&, float=0);
    void GaussianBlur(Object&, Object&, float=1, float=1, float=0);
    void MeanBox(Object&, Object&, float=1, float=1, float=0);
    void MaximumBox(Object&, Object&, float=1, float=1, float=0);
    void MinimumBox(Object&, Object&, float=1, float=1, float=0);
    void Greater(Object&, Object&, Object&);
    void GreaterOrEqual(Object&, Object&, Object&);
    void GreaterConstant(Object&, Object&, float=0);
    void GreaterOrEqualConstant(Object&, Object&, float=0);
    void MaximumZProjection(Object&, Object&);
    void MaximumYProjection(Object&, Object&);
    void MaximumXProjection(Object&, Object&);
    void MaximumOfAllPixels(Object&, Object&);
    void MinimumZProjection(Object&, Object&);
    void MinimumYProjection(Object&, Object&);
    void MinimumXProjection(Object&, Object&);
    void MinimumOfAllPixels(Object&, Object&);
    void MeanSphere(Object&, Object&, int=1, int=1, int=0);
    void NonzeroMinimumBox(Object&, Object&, Object&);
    void NotEqual(Object&, Object&, Object&);
    void NotEqualConstant(Object&, Object&, float=0);
    void Absolute(Object&, Object&);
    void Sobel(Object&, Object&);
    void Set(Object&, float=0);
    void SetNonzeroPixelsToPixelindex(Object&, Object&);
    void DifferenceOfGaussian(Object&, Object&, float=0, float=0, float=0, float=1, float=1, float=0);
    void Smaller(Object&, Object&, Object&);
    void SmallerOrEqual(Object&, Object&, Object&);
    void SmallerConstant(Object&, Object&, float=0);
    void SmallerOrEqualConstant(Object&, Object&, float=0);
    void Copy(Object&, Object&);
    void DetectMaximaBox(Object&, Object&);
    void SumZProjection(Object&, Object&);
    void SumYProjection(Object&, Object&);
    void SumXProjection(Object&, Object&);
    void SumOfAllPixels(Object&, Object&);
    void ConnectedComponentLabellingBox(Object&, Object&);
    void ReplaceIntensity(Object&, Object&, float=0, float=0);
    void ReplaceIntensities(Object&, Object&, Object&);
    void SetColumn(Object&, int=0, float=0);
    void SumReductionX(Object&, Object&, int=0);
    void BlockEnumerate(Object&, Object&, Object&, int=0);  //! block enumarate fail when running with Image
    void FlagExistingLabels(Object&, Object&);
    void CloseIndexGapsInLabelMap(Object&, Object&, int=4096);
};

    template<class T>
    const cle::Object Clesperanto::Create(const std::array<size_t,3>& t_shape, const std::string& t_type) const
    {
        return this->m_gpu->Create<T>(t_shape, t_type);
    }

    template<class T>
    const cle::Object Clesperanto::Push(const std::vector<T>& t_array, const std::array<size_t,3>& t_shape, const std::string& t_type) const
    {
        return this->m_gpu->Push<T>(t_array, t_shape, t_type);
    }

    template<class T>
    const std::vector<T> Clesperanto::Pull(const cle::Object& t_object) const
    {
        return this->m_gpu->Pull<T>(t_object);
    }

} // namespace cle

#endif //__clesperanto_hpp