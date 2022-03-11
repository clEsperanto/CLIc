
#ifndef __cleDifferenceOfGaussianKernel_hpp
#define __cleDifferenceOfGaussianKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DifferenceOfGaussianKernel : public Kernel
{
public:
    DifferenceOfGaussianKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSigma1(float=1, float=1, float=1);
    void SetSigma2(float=2, float=2, float=2);
    void Execute();

private:
    float m_Sigma1[3] = {1, 1, 1};
    float m_Sigma2[3] = {2, 2, 2};
};

} // namespace cle

#endif // __cleDifferenceOfGaussianKernel_hpp
