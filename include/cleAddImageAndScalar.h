/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleAddImageAndScalar_h
#define __cleAddImageAndScalar_h

#include "cleKernel.h"

namespace cle
{
    
class AddImageAndScalar : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    AddImageAndScalar(GPU& gpu) : Kernel(gpu) {kernelName = "add_image_and_scalar";}

    void Execute(Buffer&, Buffer&, float);

    ~AddImageAndScalar(){};
};

} // namespace cle

#endif // __cleAddImageAndScalar_h
