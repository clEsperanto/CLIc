/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleFlagExistingIntensitiesKernel_h
#define __cleFlagExistingIntensitiesKernel_h

#include "cleKernel.h"

namespace cle
{
    
class FlagExistingIntensitiesKernel : public Kernel
{
private:

public:
    FlagExistingIntensitiesKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "flag_existing_intensities";
        tagList = {"dst" , "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~FlagExistingIntensitiesKernel() = default;
};

} // namespace cle

#endif // __cleFlagExistingIntensitiesKernel_h
