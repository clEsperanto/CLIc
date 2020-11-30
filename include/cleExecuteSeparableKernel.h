/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleExecuteSeparableKernel_h
#define __cleExecuteSeparableKernel_h

#include "cleKernel.h"

namespace cle
{

class ExecuteSeparableKernel : public Kernel
{
private:
    int dimension = 1;
    std::array<float,3> sigma;
    std::array<int,3> kernel_size;

protected: 

    int Sigma2KernelSize(float);

public:
    ExecuteSeparableKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "_separable";
        tagList = {"dst" , "src", "dim", "N", "s"};
    }

    void SetKernelName(std::string);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSigma(float, float, float);
    void Execute();

    ~ExecuteSeparableKernel() = default;
};

} // namespace cle

#endif // __cleExecuteSeparableKernel_h
