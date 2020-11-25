/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSetNonzeroPixelsToPixelindexKernel_h
#define __cleSetNonzeroPixelsToPixelindexKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SetNonzeroPixelsToPixelindexKernel : public Kernel
{

public:
    SetNonzeroPixelsToPixelindexKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "set_nonzero_pixels_to_pixelindex";
        tagList = {"dst" , "src"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();

    ~SetNonzeroPixelsToPixelindexKernel() = default;
};

} // namespace cle

#endif // __cleSetNonzeroPixelsToPixelindexKernel_h
