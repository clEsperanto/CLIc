
#ifndef __cleGaussianBlurKernel_h
#define __cleGaussianBlurKernel_h

#include "cleKernel.h"

namespace cle
{
    
class GaussianBlurKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

    float Sigma2KernelSize(float);

public:
    GaussianBlurKernel(GPU& gpu) : 
        Kernel( gpu,
                "gaussian_blur",
                {"dst", "src"}
            )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

};

} // namespace cle

#endif // __cleGaussianBlurKernel_h
