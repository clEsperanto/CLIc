/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __clmaximumzprojection_h
#define __clmaximumzprojection_h

#include "clkernel.h"


class clMaximumZProjection : public clKernel
{
private:
    const std::string kernelName = "maximum_z_projection";
    std::string input_tag = "src";
    std::string output_tag = "dst_max";

public:
    // clMaximumZProjection(clGPU&, clBuffer&, clBuffer&);

    void Execute(clBuffer&, clBuffer&);

    ~clMaximumZProjection(){};
};

#endif // __clmaximumzprojection_h
