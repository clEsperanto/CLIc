/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleMaximumYProjectionKernel.h"

namespace cle
{
    
void MaximumYProjectionKernel::SetInput(Object& x)
{
    objectList.insert({"src", x});
}

void MaximumYProjectionKernel::SetOutput(Object& x)
{
    objectList.insert({"dst_max", x});
}

void MaximumYProjectionKernel::Execute()
{
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
