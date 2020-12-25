/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSmallerOrEqualConstantKernel_h
#define __cleSmallerOrEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerOrEqualConstantKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    SmallerOrEqualConstantKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "smaller_or_equal_constant";
        tagList = {"src1" , "scalar", "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetConstant(float);

    ~SmallerOrEqualConstantKernel() = default;

    void Execute();

};

} // namespace cle

#endif // __cleSmallerOrEqualConstantKernel_h
