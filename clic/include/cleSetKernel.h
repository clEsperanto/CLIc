/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSetKernel_h
#define __cleSetKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SetKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    SetKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "set";
        tagList = {"dst", "value"};
    }

    void SetInput(Object&);
    void SetValue(float);
    void Execute();

    ~SetKernel() = default;
};

} // namespace cle

#endif // __cleSetKernel_h
