/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMaximumOfAllPixelsKernel_h
#define __cleMaximumOfAllPixelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumOfAllPixelsKernel : public Kernel
{
private:

public:
    MaximumOfAllPixelsKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "maximum_of_all_pixels";
        tagList = {"dst_max", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~MaximumOfAllPixelsKernel() = default;


};

} // namespace cle

#endif // __cleMaximumOfAllPixelsKernel_h
