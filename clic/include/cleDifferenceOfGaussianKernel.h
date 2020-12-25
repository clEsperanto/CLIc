/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


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
    DifferenceOfGaussianKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "add_image_and_scalar";
        tagList = {"src", "dst", "scalar"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSigma1(float, float, float);
    void SetSigma2(float, float, float);
    void Execute();

    ~DifferenceOfGaussianKernel() = default;
};

} // namespace cle

#endif // __cleDifferenceOfGaussianKernel_h
