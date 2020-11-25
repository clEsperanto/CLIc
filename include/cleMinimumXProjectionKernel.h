/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMinimumXProjectionKernel_h
#define __cleMinimumXProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MinimumXProjectionKernel : public Kernel
{
private:

public:
    MinimumXProjectionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "minimum_x_projection";
        tagList = {"dst_min", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~MinimumXProjectionKernel() = default;


};

} // namespace cle

#endif // __cleMinimumXProjectionKernel_h
