/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __claddimageandscalar_h
#define __claddimageandscalar_h

#include "clkernel.h"


class clAddImageAndScalar : public clKernel
{
private:
    const std::string kernelName = "add_image_and_scalar_2d";
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    // clAddImageAndScalar(clGPU&, clBuffer&, clBuffer&, float);

    void Execute(clBuffer&, clBuffer&, float);

    ~clAddImageAndScalar(){};
};

#endif // __claddimageandscalar_h
