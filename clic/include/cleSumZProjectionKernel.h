/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSumZProjectionKernel_h
#define __cleSumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumZProjectionKernel : public Kernel
{
private:

public:
    SumZProjectionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "sum_z_projection";
        tagList = {"dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~SumZProjectionKernel() = default;


};

} // namespace cle

#endif // __cleSumZProjectionKernel_h
