/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __clmaximumzprojection_h
#define __clmaximumzprojection_h

#include "clkernel.h"


class clMaximumZProjection : public clKernel
{
private:
    std::string input_tag = "src";
    std::string output_tag = "dst_max";

public:
    clMaximumZProjection(clGPU& gpu) : clKernel(gpu) {kernelName = "maximum_z_projection";}
    ~clMaximumZProjection(){};

    void Execute(clBuffer&, clBuffer&);

};

#endif // __clmaximumzprojection_h
