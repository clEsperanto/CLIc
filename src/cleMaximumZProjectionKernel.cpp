/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleMaximumZProjectionKernel.h"

namespace cle
{
    
void MaximumZProjectionKernel::SetInput(Object& x)
{
    objectList.insert({"src", x});
}

void MaximumZProjectionKernel::SetOutput(Object& x)
{
    objectList.insert({"dst_max", x});
}

void MaximumZProjectionKernel::Execute()
{
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
