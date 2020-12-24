/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSumOfAllPixelsKernel_h
#define __cleSumOfAllPixelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SumOfAllPixelsKernel : public Kernel
{

public:
    SumOfAllPixelsKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "sum_of_all_pixels";
        tagList = {"dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~SumOfAllPixelsKernel() = default;


};

} // namespace cle

#endif // __cleSumOfAllPixelsKernel_h
