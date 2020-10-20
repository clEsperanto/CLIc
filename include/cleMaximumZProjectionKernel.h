/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMaximumZProjectionKernel_h
#define __cleMaximumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumZProjectionKernel : public Kernel
{
private:

public:
    MaximumZProjectionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "maximum_z_projection";
        tagList = {"dst_max", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~MaximumZProjectionKernel() = default;


};

} // namespace cle

#endif // __cleMaximumZProjectionKernel_h
