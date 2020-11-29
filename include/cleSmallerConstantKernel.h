/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSmallerConstantKernel_h
#define __cleSmallerConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerConstantKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    SmallerConstantKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "smaller_constant";
        tagList = {"src1" , "scalar", "dst"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetConstant(float);

    ~SmallerConstantKernel() = default;

    void Execute();

};

} // namespace cle

#endif // __cleSmallerConstantKernel_h
