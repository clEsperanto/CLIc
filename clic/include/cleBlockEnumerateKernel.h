/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleBlockEnumerateKernel_h
#define __cleBlockEnumerateKernel_h

#include "cleKernel.h"

namespace cle
{
    
class BlockEnumerateKernel : public Kernel
{
private:

public:
    BlockEnumerateKernel(GPU& gpu) : Kernel(gpu) 
    {
        kernelName = "block_enumerate";
        tagList = {"dst", "src", "src_sums", "blocksize"};
    }

    void SetInput(Object&);
    void SetInputSums(Object&);
    void SetOutput(Object&);
    void SetBlocksize(int);
    void Execute();

    ~BlockEnumerateKernel() = default;
};

} // namespace cle

#endif // __cleBlockEnumerateKernel_h
