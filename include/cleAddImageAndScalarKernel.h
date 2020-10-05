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
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    AddImageAndScalarKernel(GPU& gpu) : Kernel(gpu) {kernelName = "add_image_and_scalar";}

    void Execute(Buffer&, Buffer&, float);

    ~AddImageAndScalarKernel(){};
};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_h
