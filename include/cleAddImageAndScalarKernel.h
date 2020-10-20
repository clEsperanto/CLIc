/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleAddImageAndScalarKernel_h
#define __cleAddImageAndScalarKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AddImageAndScalarKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    AddImageAndScalarKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "add_image_and_scalar";
        tagList = {"src", "dst", "scalar"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetScalar(float&);
    void Execute();

    ~AddImageAndScalarKernel() = default;
};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_h
