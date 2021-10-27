
#ifndef __cleGaussianBlurKernel_hpp
#define __cleGaussianBlurKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GaussianBlurKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_gaussian_blur_separable_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_gaussian_blur_separable_3d.h" 
        ;

public:
    GaussianBlurKernel(std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

private:
    float m_x;
    float m_y;
    float m_z;
    int Sigma2KernelSize(float) const;
};

} // namespace cle

#endif // __cleGaussianBlurKernel_hpp
