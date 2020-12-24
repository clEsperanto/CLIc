/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSeparableKernel_h
#define __cleSeparableKernel_h

#include "cleKernel.h"

namespace cle
{

class SeparableKernel : public Kernel
{
private:
    void DefineDimensionality();

public:
    SeparableKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "";
        tagList = {"dst" , "src", "dim", "N", "s"};
    }

    void SetKernelName(std::string);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSize(int);
    void SetSigma(float);
    void SetDimension(int);
    void Execute();

    ~SeparableKernel() = default;
};

} // namespace cle

#endif // __cleSeparableKernel_h
