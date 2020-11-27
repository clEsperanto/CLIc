/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSmallerKernel_h
#define __cleSmallerKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    SmallerKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "smaller";
        tagList = {"src1" , "src2", "dst"};
    }

    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void Set(float);

    ~SmallerKernel() = default;

    void Execute();

};

} // namespace cle

#endif // __cleSmallerKernel_h
