/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMeanKernel_h
#define __cleMeanKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MeanKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

public:
    MeanKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "mean";
        tagList = {"dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSigma(float=0, float=0, float=0);
    void Execute();

    ~MeanKernel() = default;
};

} // namespace cle

#endif // __cleMeanKernel_h
