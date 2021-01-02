/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleReplaceIntensityKernel.h"

namespace cle
{

void ReplaceIntensityKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void ReplaceIntensityKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void ReplaceIntensityKernel::SetInValue(float x)
{
    Float* val = new Float(x);
    this->AddObject(val, "in");
}

void ReplaceIntensityKernel::SetOutValue(float x)
{
    Float* val = new Float(x);
    this->AddObject(val, "out");
}

void ReplaceIntensityKernel::Execute()
{
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
