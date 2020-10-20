/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/

#include "cleMean2DSphereKernel.h"

namespace cle
{
    
void Mean2DSphereKernel::ComputeKernelSize()
{
    for (auto it = intList.begin(); it != intList.end(); it++)
    {
        if (std::find(tagList.begin(), tagList.end(), it->first) != tagList.end())
        {
            it->second = it->second * 2 + 1;
        }
    } 
}

void Mean2DSphereKernel::SetInput(Object& x)
{
    objectList.insert({"src", x});
}

void Mean2DSphereKernel::SetOutput(Object& x)
{
    objectList.insert({"dst", x});
}

void Mean2DSphereKernel::SetRadiusX(int& x)
{
    intList.insert({"radius_x", x});
}

void Mean2DSphereKernel::SetRadiusY(int& x)
{
    intList.insert({"radius_y", x});
}

void Mean2DSphereKernel::Execute()
{
    CompileKernel();
    ComputeKernelSize();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
