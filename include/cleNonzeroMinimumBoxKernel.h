/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleNonzeroMinimumBoxKernel_h
#define __cleNonzeroMinimumBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class NonzeroMinimumBoxKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    NonzeroMinimumBoxKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "nonzero_minimum_box";
        tagList = {"dst", "flag_dst", "src"};
    }

    void SetInput(Object&);
    void SetOutput1(Object&);
    void SetOutput2(Object&);
    void Execute();

    ~NonzeroMinimumBoxKernel() = default;
};

} // namespace cle

#endif // __cleNonzeroMinimumBoxKernel_h
