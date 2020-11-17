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

}

void Mean2DSphereKernel::SetInput(Object& x)
{
    this->AddObject(x, "src");
}

void Mean2DSphereKernel::SetOutput(Object& x)
{
    this->AddObject(x, "dst");
}

void Mean2DSphereKernel::SetRadiusX(int& x)
{
    Int val(x * 2 + 1);
    this->AddObject(val, "radius_x");}

void Mean2DSphereKernel::SetRadiusY(int& x)
{
    Int val(x * 2 + 1);
    this->AddObject(val, "radius_y");}

void Mean2DSphereKernel::Execute()
{
    CompileKernel();
    ComputeKernelSize();
    AddArgumentsToKernel();
    DefineRangeKernel();
}

} // namespace cle
