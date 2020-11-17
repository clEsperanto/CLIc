/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleSmallerOrEqualConstantKernel.h"

namespace cle
{

void SmallerOrEqualConstantKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src1"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
}

void SmallerOrEqualConstantKernel::SetInput(Object& x)
{
    this->AddObject(x, "src1");
}

void SmallerOrEqualConstantKernel::SetOutput(Object& x)
{
    this->AddObject(x, "dst");
}

void SmallerOrEqualConstantKernel::SetConstant(float& x)
{
    Float val(x);
    this->AddObject(val, "scalar");
}

void SmallerOrEqualConstantKernel::Execute()
{
    DefineDimensionality();
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
