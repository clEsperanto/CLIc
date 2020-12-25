/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/

#ifndef __cleMean2DSphereKernel_h
#define __cleMean2DSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class Mean2DSphereKernel : public Kernel
{
private:

    void ComputeKernelSize();

public:
    Mean2DSphereKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "mean_sphere_2d";
        tagList = {"dst", "src", "radius_x", "radius_y"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadiusX(int);
    void SetRadiusY(int);
    void Execute();

    ~Mean2DSphereKernel() = default;
};

} // namespace cle

#endif // __cleMean2DSphereKernel_h
