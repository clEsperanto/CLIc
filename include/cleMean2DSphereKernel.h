/**
 * Author: Robert Haase - @haesleinhuepf
 *         Stephane Rigaud - @strigaud 
 */

#ifndef __cleMean2DSphereKernel_h
#define __cleMean2DSphereKernel_h

#include "cleKernel.h"

namespace cle
{
    
class Mean2DSphereKernel : public Kernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst";

public:
    Mean2DSphereKernel(GPU& gpu) : Kernel(gpu) {kernelName = "mean_sphere";}

    void Execute(Buffer&, Buffer&, int, int);

    ~Mean2DSphereKernel(){};
};

} // namespace cle

#endif // __cleMean2DSphereKernel_h
