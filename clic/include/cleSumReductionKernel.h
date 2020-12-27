/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSumReductionKernel_h
#define __cleSumReductionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumReductionKernel : public Kernel
{
private:

public:
    SumReductionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "sum_reduction";
        tagList = {"dst", "src", "blocksize"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetBlocksize(int);
    void Execute();

    ~SumReductionKernel() = default;
};

} // namespace cle

#endif // __cleSumReductionKernel_h
