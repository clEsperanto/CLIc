/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleSmallerOrEqualConstantKernel_h
#define __cleSmallerOrEqualConstantKernel_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerOrEqualConstantKernel : public Kernel
{
private:
    std::string input_tag = "src1";
    std::string output_tag = "dst";

public:
    SmallerOrEqualConstantKernel(GPU& gpu) : Kernel(gpu) {kernelName = "smaller_or_equal_constant";}
    ~SmallerOrEqualConstantKernel(){};

    void Execute(Buffer& in, Buffer& out, float scalar);

};

} // namespace cle

#endif // __cleSmallerOrEqualConstantKernel_h
