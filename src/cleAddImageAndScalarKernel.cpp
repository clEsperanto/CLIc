/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleAddImageAndScalarKernel.h"

namespace cle
{
    
void AddImageAndScalarKernel::DefineDimensionality()
{
    std::string dim = "_2d";
    for (auto it = objectList.begin(); it != objectList.end(); it++)
    {
        if (it->second.GetDimensions()[2] > 1)
        {
            dim = "_3d";
        }
    }
    kernelName = kernelName + dim;
}

void AddImageAndScalarKernel::SetInput(Object& x)
{
    objectList.insert({"src", x});
}

void AddImageAndScalarKernel::SetOutput(Object& x)
{
    objectList.insert({"dst", x});
}

void AddImageAndScalarKernel::SetScalar(float& x)
{
    floatList.insert({"scalar", x});
}

void AddImageAndScalarKernel::Execute()
{
    DefineDimensionality();
    CompileKernel();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
