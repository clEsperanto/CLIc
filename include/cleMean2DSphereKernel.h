/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/

#ifndef __cleMean2DSphereKernel_h
#define __cleMean2DSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class Mean2DSphereKernel : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    Mean2DSphereKernel(GPU& gpu) : Kernel(gpu) {kernelName = "mean_sphere";}

    void Execute(Buffer&, Buffer&, int, int);

    ~Mean2DSphereKernel(){};
};

} // namespace cle

#endif // __cleMean2DSphereKernel_h
