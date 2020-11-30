/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleLabelConnectComponentBoxKernel_h
#define __cleLabelConnectComponentBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class LabelConnectComponentBoxKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    LabelConnectComponentBoxKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "label_connect_component";
        tagList = {"src" , "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~LabelConnectComponentBoxKernel() = default;
};

} // namespace cle

#endif // __cleLabelConnectComponentBoxKernel_h
