/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleMaximumXProjectionKernel.h"

namespace cle
{
    
void MaximumXProjectionKernel::SetInput(Object& x)
{
    objectList.insert({"src", x});
}

void MaximumXProjectionKernel::SetOutput(Object& x)
{
    objectList.insert({"dst_max", x});
}

void MaximumXProjectionKernel::Execute()
{
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
