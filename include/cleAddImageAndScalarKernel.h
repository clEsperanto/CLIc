/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleAddImageAndScalarKernel_h
#define __cleAddImageAndScalarKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AddImageAndScalarKernel : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    AddImageAndScalarKernel(GPU& gpu) : Kernel(gpu) {kernelName = "add_image_and_scalar";}

    void Execute(Buffer&, Buffer&, float);

    ~AddImageAndScalarKernel(){};
};

} // namespace cle

#endif // __cleAddImageAndScalarKernel_h
