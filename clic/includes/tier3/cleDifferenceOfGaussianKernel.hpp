
#ifndef __cleDifferenceOfGaussianKernel_hpp
#define __cleDifferenceOfGaussianKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DifferenceOfGaussianKernel : public Kernel
{
private:

    float sigma1[3] = {1, 1, 1};
    float sigma2[3] = {2, 2, 2};

public:
    DifferenceOfGaussianKernel(std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma1(float, float, float);
    void SetSigma2(float, float, float);
    void Execute();

};

} // namespace cle

#endif // __cleDifferenceOfGaussianKernel_hpp
