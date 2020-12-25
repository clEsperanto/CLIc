/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleNonzeroMinimumBoxKernel.h"

namespace cle
{

void NonzeroMinimumBoxKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    Buffer* bufferObject = dynamic_cast<Buffer*>(parameterList.at("src"));
    if(bufferObject->GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    kernelName = kernelName + dim;
}

void NonzeroMinimumBoxKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void NonzeroMinimumBoxKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void NonzeroMinimumBoxKernel::SetOutputFlag(Object& x)
{
    this->AddObject(&x, "flag_dst");
}

void NonzeroMinimumBoxKernel::Execute()
{
    std::cout << "A" << std::endl;
    DefineDimensionality();
    std::cout << "b" << std::endl;
    CompileKernel();
    std::cout << "c" << std::endl;
    AddArgumentsToKernel();
    std::cout << "d" << std::endl;
    DefineRangeKernel();
    std::cout << "e" << std::endl;
}

} // namespace cle
