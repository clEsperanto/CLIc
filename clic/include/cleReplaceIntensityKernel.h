/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleReplaceIntensityKernel_h
#define __cleReplaceIntensityKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ReplaceIntensityKernel : public Kernel
{
private:

public:
    ReplaceIntensityKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "replace_intensity";
        tagList = {"dst", "src", "in", "out"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetInValue(float);
    void SetOutValue(float);
    void Execute();

    ~ReplaceIntensityKernel() = default;
};

} // namespace cle

#endif // __cleReplaceIntensityKernel_h
