/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleAddImagesWeightedKernel_h
#define __cleAddImagesWeightedKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AddImagesWeightedKernel : public Kernel
{
private:

    void DefineDimensionality();

public:
    AddImagesWeightedKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "add_images_weighted";
        tagList = {"src", "src1", "dst", "factor", "factor1"};
    }

    void SetInput1(Object&);
    void SetInput2(Object&);
    void SetOutput(Object&);
    void SetFactor1(float);
    void SetFactor2(float);
    void Execute();

    ~AddImagesWeightedKernel() = default;
};

} // namespace cle

#endif // __cleAddImagesWeightedKernel_h
