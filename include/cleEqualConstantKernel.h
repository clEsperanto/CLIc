/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleEqualConstantKernel_h
#define __cleEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class EqualConstantKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    EqualConstantKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "equal_constant";
        tagList = {"src1", "scalar", "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();

    ~EqualConstantKernel() = default;
};

} // namespace cle

#endif // __cleEqualConstantKernel_h
