/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleFlagExistingLabelsKernel_h
#define __cleFlagExistingLabelsKernel_h

#include "cleKernel.h"

namespace cle
{
    
class FlagExistingLabelsKernel : public Kernel
{
private:

public:
    FlagExistingLabelsKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "flag_existing_labels";
        tagList = {"dst" , "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~FlagExistingLabelsKernel() = default;
};

} // namespace cle

#endif // __cleFlagExistingLabelsKernel_h
