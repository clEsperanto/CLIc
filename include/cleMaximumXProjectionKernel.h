/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMaximumXProjectionKernel_h
#define __cleMaximumXProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumXProjectionKernel : public Kernel
{
private:

public:
    MaximumXProjectionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "maximum_x_projection";
        tagList = {"dst_max", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~MaximumXProjectionKernel() = default;


};

} // namespace cle

#endif // __cleMaximumXProjectionKernel_h
