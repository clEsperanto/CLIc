/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleSmallerOrEqualConstant_h
#define __cleSmallerOrEqualConstant_h

#include "cleKernel.h"

namespace cle
{
    
class SmallerOrEqualConstant : public Kernel
{
private:
    std::string input_tag = "src1";
    std::string output_tag = "dst";

public:
    SmallerOrEqualConstant(GPU& gpu) : Kernel(gpu) {kernelName = "smaller_or_equal_constant";}
    ~SmallerOrEqualConstant(){};

    void Execute(Buffer& in, Buffer& out, float scalar);

};

} // namespace cle

#endif // __cleSmallerOrEqualConstant_h
