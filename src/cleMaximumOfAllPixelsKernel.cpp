/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleMaximumOfAllPixelsKernel.h"
#include "cleMaximumZProjectionKernel.h"
#include "cleMaximumYProjectionKernel.h"
#include "cleMaximumXProjectionKernel.h"

namespace cle
{
    
void MaximumOfAllPixelsKernel::SetInput(Object& x)
{
    this->AddObject(x, "src");
}

void MaximumOfAllPixelsKernel::SetOutput(Object& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumOfAllPixelsKernel::Execute()
{
    std::cout << "MaximumOfAllPixelsKernel is not implemented yet. Sorry :)" << std::endl;
    // TODO: 
    // Create and execute a pipeline of kernels
    //      this requires the access to context, and command queue for compiling and executing kernels
    // Need to create temporary output in device mememory
    //      this requires the access to context
    // Output is a scalar
    //      this open the possibility that kernels do not return only GPU object but also native types. (not urgent...)
    // 
    // Maximum of all pixel pipeline in speudo-code
    // if src.dimension[2] > 1:
    //     tmp = create(src.dimension[0], src.dimension[1], 1)
    //     MaximumZProjection kernel()
    //     kernel.SetIntput(src)
    //     kernel.SetOutput(tmp)
    //     kernel.Execute()
    //     src = tmp
    //
    // if src.dimension[1] > 1:
    //     tmp = create(src.dimension[0], 1, 1)
    //     MaximumYProjection kernel()
    //     kernel.SetIntput(src)
    //     kernel.SetOutput(tmp)
    //     kernel.Execute()
    //     src = tmp
    //
    // MaximumXProjection kernel()
    // kernel.SetIntput(src)
    // kernel.SetOutput(dst)
    // kernel.Execute()
}

} // namespace cle
