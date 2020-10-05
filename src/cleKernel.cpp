/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleKernel.h"
#include "utils.h"

namespace cle
{
    
std::string Kernel::LoadPreamble()
{
    std::string preamble;
    std::ifstream file(preambleFile.c_str(), std::ios::in | std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        preamble.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&preamble[0], preamble.size());
        file.close();
    }
    else
    {
    std::cerr << "Error reading file! Cannot open " << preambleFile << std::endl;
    }
    return preamble;
}

std::string Kernel::LoadSources()
{
    std::string sources;
    std::string suffix = "_x.cl";
    std::string filename = kernelFolder + filesep + kernelName + suffix;
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        sources.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&sources[0], sources.size());
        file.close();
    }
    else
    {
        std::cerr << "Error reading file! Cannot open " << filename << std::endl;
    }
    return sources;
}

std::string Kernel::LoadDefines()
{
    std::string defines;
    defines = defines + "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
    defines = defines + "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
    defines = defines + "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
    defines = defines + "\n";   

    for (auto itr = parameters.begin(); itr != parameters.end(); ++itr)
    {
        // image type handling
        defines = defines + "\n#define CONVERT_" + itr->first + "_PIXEL_TYPE clij_convert_" + itr->second.GetType() + "_sat";
        defines = defines + "\n#define IMAGE_" + itr->first + "_TYPE __global " + itr->second.GetType() + "*";
        defines = defines + "\n#define IMAGE_" + itr->first + "_PIXEL_TYPE " + itr->second.GetType();

        // image size handling
        if (itr->second.GetDimensions()[2] > 1)
        {
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_WIDTH " + std::to_string(itr->second.GetDimensions()[0]);
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_HEIGHT " + std::to_string(itr->second.GetDimensions()[1]);
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_DEPTH " + std::to_string(itr->second.GetDimensions()[2]);
        }
        else
        {
            if (itr->second.GetDimensions()[1] > 1)
            {
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_WIDTH " + std::to_string(itr->second.GetDimensions()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_HEIGHT " + std::to_string(itr->second.GetDimensions()[1]);
            }
            else
            {
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_WIDTH " + std::to_string(itr->second.GetDimensions()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_HEIGHT 1";
            }
            defines = defines + "\n#define IMAGE_SIZE_" + itr->first + "_DEPTH 1";
        }

        // position (dimensionality) handling
        if (itr->second.GetDimensions()[2] == 1)
        {
            defines = defines + "\n#define POS_" + itr->first + "_TYPE int2";
            if (itr->second.GetDimensions()[1] == 1) // 1D
            {
                defines = defines + "\n#define POS_" + itr->first + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, 0)";
            }
            else // 2D
            {
                defines = defines + "\n#define POS_" + itr->first + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, pos1)";
            }
        }
        else // 3/4D
        {
            defines = defines + "\n#define POS_" + itr->first + "_TYPE int4";
            defines =
                defines + "\n#define POS_" + itr->first + "_INSTANCE(pos0,pos1,pos2,pos3) (int4)(pos0, pos1, pos2, 0)";
        }

        // read/write images
        std::string sdim = (itr->second.GetDimensions()[2] == 1) ? "2" : "3";
        defines = defines + "\n#define READ_" + itr->first + "_IMAGE(a,b,c) read_buffer" + sdim + "d" + itr->second.GetTypeId() +
                "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
        defines = defines + "\n#define WRITE_" + itr->first + "_IMAGE(a,b,c) write_buffer" + sdim + "d" + itr->second.GetTypeId() +
                "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
        defines = defines + "\n";
    }
    return defines;
}

std::string Kernel::DefineDimensionality(Buffer& data)
{
    std::string dim = "";
    if (data.GetDimensions()[2] > 1)
    {
        dim = "_3d";
    }
    else
    {
        dim = "_2d";
    }
    return dim;
}

void Kernel::CompileKernel()
{
    kernelName = kernelName + dimensionality;

    // read kernel, defines, and preamble
    std::string kernel_src = LoadSources();
    std::string defines_src = LoadDefines();
    std::string preambule_src = LoadPreamble();

    // construct final source code
    std::string ocl_src = defines_src + "\n" + preambule_src + "\n" + kernel_src;
    const char *source_str = (ocl_src).c_str();
    size_t source_size = (ocl_src).size();  

    // Create a program from the kernel source
    cl_int clError;
    program = clCreateProgramWithSource(context, 1, &source_str, &source_size, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Program error! Fail to create program : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    // build the program
    clError = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Program error! Fail to build program : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    // create the OpenCL kernel
    kernel = clCreateKernel(program, kernelName.c_str(), &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Program error! Fail to create kernel in maximumzprojection() : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

std::string Kernel::GetKernelName()
{
    return kernelName;
}

cl_kernel Kernel::GetKernel()
{
    return kernel;
}

cl_program Kernel::GetProgram()
{
    return program;
}

cl_device_id Kernel::GetDevice()
{
    return device_id;
}

cl_context Kernel::GetContext()
{
    return context;
}

cl_command_queue Kernel::GetCommandQueue()
{
    return command_queue;
}

Kernel::Kernel(GPU& gpu)
{
    this->device_id = gpu.GetDevice();
    this->context = gpu.GetContext();
    this->command_queue = gpu.GetCommandQueue();
}

} // namespace cle
