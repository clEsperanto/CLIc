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
    this->AddObject(&x, "src");
}

void MaximumOfAllPixelsKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst_max");
}

void MaximumOfAllPixelsKernel::Execute()
{
    std::cout << "MaximumOfAllPixelsKernel is not implemented yet. Sorry :)" << std::endl;
}

} // namespace cle
