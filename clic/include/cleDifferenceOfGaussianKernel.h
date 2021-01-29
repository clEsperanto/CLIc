
#ifndef __cleDifferenceOfGaussianKernel_h
#define __cleDifferenceOfGaussianKernel_h

#include "cleKernel.h"

namespace cle
{
    
class DifferenceOfGaussianKernel : public Kernel
{
private:

    float sigma1[3] = {1, 1, 1};
    float sigma2[3] = {2, 2, 2};

public:
    DifferenceOfGaussianKernel(GPU& gpu) :
        Kernel( gpu,
                "difference_of_gaussians",
                {"src", "dst"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma1(float, float, float);
    void SetSigma2(float, float, float);
    void Execute();

};

} // namespace cle

#endif // __cleDifferenceOfGaussianKernel_h
