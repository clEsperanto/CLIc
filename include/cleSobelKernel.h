/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSobelKernel_h
#define __cleSobelKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SobelKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    SobelKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "sobel";
        tagList = {"dst" , "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~SobelKernel() = default;
};

} // namespace cle

#endif // __cleSobelKernel_h
