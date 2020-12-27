/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleConnectedComponentsLabelingBoxKernel_h
#define __cleConnectedComponentsLabelingBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class ConnectedComponentsLabelingBoxKernel : public Kernel
{
private:


public:
    ConnectedComponentsLabelingBoxKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "ConnectedComponentsLabelingBox";
        tagList = {"src" , "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~ConnectedComponentsLabelingBoxKernel() = default;
};

} // namespace cle

#endif // __cleConnectedComponentsLabelingBoxKernel_h
