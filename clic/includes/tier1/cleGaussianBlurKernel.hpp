
#ifndef __cleGaussianBlurKernel_hpp
#define __cleGaussianBlurKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GaussianBlurKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_gaussian_blur_separable_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_gaussian_blur_separable_3d.h" 
        ;

public:
    GaussianBlurKernel (std::shared_ptr<GPU> gpu);

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

private:
    float x;
    float y;
    float z;
    float Sigma2KernelSize(float);
};

} // namespace cle

#endif // __cleGaussianBlurKernel_hpp
