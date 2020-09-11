/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __clsmallerorequalconstant_h
#define __clsmallerorequalconstant_h

#include "clkernel.h"


class clSmallerOrEqualConstant : public clKernel
{
private:
    std::string input_tag = "src1";
    std::string output_tag = "dst";

public:
    clSmallerOrEqualConstant(clGPU& gpu) : clKernel(gpu) {kernelName = "smaller_or_equal_constant";}
    ~clSmallerOrEqualConstant(){};

    void Execute(clBuffer& in, clBuffer& out, float scalar);

};

#endif // __clsmallerorequalconstant_h
