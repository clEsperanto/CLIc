/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleMaximumZProjection_h
#define __cleMaximumZProjection_h

#include "cleKernel.h"

namespace cle
{
    
class MaximumZProjection : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst_max";

public:
    MaximumZProjection(GPU& gpu) : Kernel(gpu) {kernelName = "maximum_z_projection";}
    ~MaximumZProjection(){};

    void Execute(Buffer&, Buffer&);

};

} // namespace cle

#endif // __cleMaximumZProjection_h
