
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
    GaussianBlurKernel(GPU& gpu) : 
        Kernel( gpu,
                "gaussian_blur_separable",
                {"dst", "src"}
            )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }

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
