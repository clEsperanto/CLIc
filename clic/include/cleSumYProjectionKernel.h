/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSumYProjectionKernel_h
#define __cleSumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumYProjectionKernel : public Kernel
{
private:

public:
    SumYProjectionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "sum_y_projection";
        tagList = {"dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~SumYProjectionKernel() = default;


};

} // namespace cle

#endif // __cleSumYProjectionKernel_h
