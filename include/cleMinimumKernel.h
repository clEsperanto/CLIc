/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMinimumKernel_h
#define __cleMinimumKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

public:
    MinimumKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "minimum";
        tagList = {"dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

    ~MinimumKernel() = default;
};

} // namespace cle

#endif // __cleMinimumKernel_h
