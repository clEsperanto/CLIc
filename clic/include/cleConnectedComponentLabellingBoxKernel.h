/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleConnectedComponentLabellingBoxKernel_h
#define __cleConnectedComponentLabellingBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ConnectedComponentLabellingBoxKernel : public Kernel
{
private:


public:
    ConnectedComponentLabellingBoxKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "ConnectedComponentsLabelingBox";
        tagList = {"src" , "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~ConnectedComponentLabellingBoxKernel() = default;
};

} // namespace cle

#endif // __cleConnectedComponentLabellingBoxKernel_h
