/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMinimumOfAllPixelsKernel_h
#define __cleMinimumOfAllPixelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumOfAllPixelsKernel : public Kernel
{
private:

public:
    MinimumOfAllPixelsKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "minimum_of_all_pixels";
        tagList = {"dst_min", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~MinimumOfAllPixelsKernel() = default;


};

} // namespace cle

#endif // __cleMinimumOfAllPixelsKernel_h
