/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __claddimageandscalar_h
#define __claddimageandscalar_h

#include "clkernel.h"


class clAddImageAndScalar : public clKernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    clAddImageAndScalar(clGPU& gpu) : clKernel(gpu) {kernelName = "add_image_and_scalar";}

    void Execute(clBuffer&, clBuffer&, float);

    ~clAddImageAndScalar(){};
};

#endif // __claddimageandscalar_h
