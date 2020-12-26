/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleSetColumnKernel_h
#define __cleSetColumnKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SetColumnKernel : public Kernel
{
private:

    void DefineDimensionality();


public:
    SetColumnKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "set_column";
        tagList = {"dst" , "column", "value"};
    }

    void SetInput(Object&);
    void SetColumn(int);
    void SetValue(float);
    void Execute();

    ~SetColumnKernel() = default;
};

} // namespace cle

#endif // __cleSetColumnKernel_h
