/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleAbsoluteKernel_h
#define __cleAbsoluteKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    AbsoluteKernel(GPU& gpu) : Kernel(gpu) {kernelName = "absolute";}

    void Execute(Buffer&, Buffer&);

    ~AbsoluteKernel(){};
};

} // namespace cle

#endif // __cleAbsoluteKernel_h
