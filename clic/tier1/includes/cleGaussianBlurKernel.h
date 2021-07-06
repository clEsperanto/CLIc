
#ifndef __cleGaussianBlurKernel_h
#define __cleGaussianBlurKernel_h

#include "cleKernel.h"

namespace cle
{
    
class GaussianBlurKernel : public Kernel
{
private:
    std::string source_1 = 
        #include "cle_gaussian_blur_separable_2d.h" 
        ;
    std::string source_2 = 
        #include "cle_gaussian_blur_separable_3d.h" 
        ;
private:
    float x;
    float y;
    float z;

    float Sigma2KernelSize(float);

public:
    GaussianBlurKernel(GPU& gpu) : 
        Kernel( gpu,
                "gaussian_blur_separable",
                {"dst", "src"}
            )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_1});
        m_Sources.insert({this->m_KernelName + "_3d", source_2});
    }

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

};

} // namespace cle

#endif // __cleGaussianBlurKernel_h
