
#ifndef __cleGaussianBlurKernel_hpp
#define __cleGaussianBlurKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GaussianBlurKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_gaussian_blur_separable.h" 
        };

public:
    GaussianBlurKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

private:
    std::array<float,3> m_Sigma = {0, 0, 0};
    std::array<int,3> Sigma2KernelSize(std::array<float,3>) const;
};

} // namespace cle

#endif // __cleGaussianBlurKernel_hpp
