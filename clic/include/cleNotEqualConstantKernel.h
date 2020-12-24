/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleNotEqualConstantKernel_h
#define __cleNotEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class NotEqualConstantKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    NotEqualConstantKernel(GPU& gpu) : Kernel(gpu)
    {
        kernelName = "not_equal_constant";
        tagList = {"src1", "scalar", "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float);
    void Execute();

    ~NotEqualConstantKernel() = default;
};

} // namespace cle

#endif // __cleNotEqualConstantKernel_h
