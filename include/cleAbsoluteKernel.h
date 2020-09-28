/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleAbsoluteKernel_h
#define __cleAbsoluteKernel_h

#include "cleKernel.h"

namespace cle
{
    
class AbsoluteKernel : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    AbsoluteKernel(GPU& gpu) : Kernel(gpu) {kernelName = "absolute";}

    void Execute(Buffer&, Buffer&);

    ~AbsoluteKernel(){};
};

} // namespace cle

#endif // __cleAbsoluteKernel_h
