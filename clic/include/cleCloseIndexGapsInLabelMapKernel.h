/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleCloseIndexGapsInLabelMapKernel_h
#define __cleCloseIndexGapsInLabelMapKernel_h

#include "cleKernel.h"

namespace cle
{
    
class CloseIndexGapsInLabelMapKernel : public Kernel
{
private:

    void DefineDimensionality();
    int blocksize = 4096;

public:
    CloseIndexGapsInLabelMapKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "close_index_gaps_in_label_map";
        tagList = {"src", "dst", "blocksize"};
    }

    void SetInput(Object&);
    void SetOutput(Object&);
    void SetBlockSize(int);
    void Execute();

    ~CloseIndexGapsInLabelMapKernel() = default;
};

} // namespace cle

#endif // __cleCloseIndexGapsInLabelMapKernel_h
