/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleMaximumYProjectionKernel_h
#define __cleMaximumYProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumYProjectionKernel : public Kernel
{
private:

public:
    MaximumYProjectionKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "maximum_y_projection";
        tagList = {"dst_max", "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~MaximumYProjectionKernel() = default;


};

} // namespace cle

#endif // __cleMaximumYProjectionKernel_h
