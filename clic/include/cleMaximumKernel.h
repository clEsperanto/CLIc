/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMaximumKernel_h
#define __cleMaximumKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

public:
    MaximumKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "maximum";
        tagList = {"dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

    ~MaximumKernel() = default;
};

} // namespace cle

#endif // __cleMaximumKernel_h
