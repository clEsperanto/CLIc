/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleCopyKernel_h
#define __cleCopyKernel_h

#include "cleKernel.h"

namespace cle
{
    
class CopyKernel : public Kernel
{
private:
    void DefineDimensionality();

public:
    CopyKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "copy";
        tagList = {"dst" , "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~CopyKernel() = default;
};

} // namespace cle

#endif // __cleCopyKernel_h
