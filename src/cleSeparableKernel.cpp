/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleSeparableKernel.h"

namespace cle
{

void SeparableKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
}

void SeparableKernel::SetKernelName(std::string name)
{
    kernelName = name;
}

void SeparableKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void SeparableKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void SeparableKernel::SetSize(int x)
{
    Int* val = new Int(x);
    this->AddObject(val, "N");
}

void SeparableKernel::SetSigma(float x)
{
    Float* val = new Float(x);
    this->AddObject(val, "s");
}

void SeparableKernel::SetDimension(int x)
{
    Int* val = new Int(x);
    this->AddObject(val, "dim");
}

void SeparableKernel::Execute()
{
    DefineDimensionality();
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
