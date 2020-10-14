/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/

#include "cleMean2DSphereKernel.h"

namespace cle
{
    
void Mean2DSphereKernel::Execute(Buffer& in, Buffer& out, int radius_x, int radius_y)
{

    std::pair<std::string, Buffer> src = std::make_pair(input_tag, in);
    std::pair<std::string, Buffer> dst = std::make_pair(output_tag, out);
    parameters.insert(src);
    parameters.insert(dst);

    dimensionality = this->DefineDimensionality(in);
    CompileKernel();

    // Set the arguments of the kernel
    cl_int clError;
    cl_mem src_mem = in.GetPointer();
    cl_mem dst_mem = out.GetPointer();
    clError = clSetKernelArg(this->GetKernel(), 0, sizeof(cl_mem), &dst_mem);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    clError = clSetKernelArg(this->GetKernel(), 1, sizeof(cl_mem), &src_mem);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    int kernel_size_x = (radius_x * 2 + 1);
    clError = clSetKernelArg(this->GetKernel(), 2, sizeof(int), (void *)&kernel_size_x);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    int kernel_size_y = (radius_y * 2 + 1);
    clError = clSetKernelArg(this->GetKernel(), 3, sizeof(int), (void*)&kernel_size_y);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }

    // execute the opencl kernel
    size_t global_item_size[2];
    for (size_t i = 0; i < 2; i++)
    {
        global_item_size[i] = std::max(in.GetDimensions()[i], out.GetDimensions()[i]);
    }
    size_t work_dim = 2;
    clError = clEnqueueNDRangeKernel(this->GetCommandQueue(), this->GetKernel(), work_dim, nullptr, global_item_size, nullptr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Execution error! Could not enqueue ND-Range : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

} // namespace cle
