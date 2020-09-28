/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleMaximumZProjectionKernel_h
#define __cleMaximumZProjectionKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumZProjectionKernel : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst_max";

public:
    MaximumZProjectionKernel(GPU& gpu) : Kernel(gpu) {kernelName = "maximum_z_projection";}
    ~MaximumZProjectionKernel(){};

    void Execute(Buffer&, Buffer&);

};

} // namespace cle

#endif // __cleMaximumZProjectionKernel_h
