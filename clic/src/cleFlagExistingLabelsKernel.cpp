/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleFlagExistingLabelsKernel.h"
#include "cleSetKernel.h"

namespace cle
{

void FlagExistingLabelsKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void FlagExistingLabelsKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void FlagExistingLabelsKernel::Execute()
{
    // Set output to 0
    Buffer *dst = dynamic_cast<Buffer*>(parameterList.at("dst"));
    SetKernel set(this->gpu);
    set.SetInput(*dst);
    set.SetValue(0);
    set.Execute();

    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
