/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include "clsmallerorequalconstant.h"

void clSmallerOrEqualConstant::Execute(clBuffer& in, clBuffer& out, float scalar)
{
    if (in.GetDimensions()[2] > 1)
    {
        dimensionality = "_3d";
    }
    else
    {
        dimensionality = "_2d";
    }

    std::pair<std::string, clBuffer> src = std::make_pair(input_tag, in);
    std::pair<std::string, clBuffer> dst = std::make_pair(output_tag, out);
    parameters.insert(src);
    parameters.insert(dst);

    CompileKernel();

    // Set the arguments of the kernel
    cl_int clError;
    cl_mem src_mem = in.GetPointer();
    cl_mem dst_mem = out.GetPointer();
    clError = clSetKernelArg(this->GetKernel(), 0, sizeof(cl_mem), &src_mem);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    clError = clSetKernelArg(this->GetKernel(), 1, sizeof(float), &scalar);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    clError = clSetKernelArg(this->GetKernel(), 2, sizeof(cl_mem), &dst_mem);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }

    // execute the opencl kernel
    size_t global_item_size[3];
    for (size_t i = 0; i < 3; i++)
    {
        global_item_size[i] = std::max(in.GetDimensions()[i], out.GetDimensions()[i]);
    }
    size_t work_dim = 3;
    clError = clEnqueueNDRangeKernel(this->GetCommandQueue(), this->GetKernel(), work_dim, nullptr, global_item_size, nullptr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Execution error! Could not enqueue ND-Range : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}