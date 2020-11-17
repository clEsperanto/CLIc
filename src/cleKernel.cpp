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

    for (auto itr = parameterList.begin(); itr != parameterList.end(); ++itr)
    {
        std::cout << "\t\ttest" << std::endl;
        std::cout << "\t\t" << itr->first;
        std::cout << " is a " << itr->second->GetObjectType() << std::endl;
        if (itr->second->IsObject("cleBuffer"))
        {    
            std::cout << "\t\t\tthis is a buffer!" << std::endl;
            Buffer* bufferObject = dynamic_cast<Buffer*>(itr->second);
            std::string tagObject = itr->first;


            std::string objectType = bufferObject->GetObjectType();
            std::string dataType = bufferObject->GetDataType();
            std::string abbrType = TypeAbbr(dataType);

            // image type handling
            defines = defines + "\n#define CONVERT_" + tagObject + "_PIXEL_TYPE clij_convert_" + dataType + "_sat";
            defines = defines + "\n#define IMAGE_" + tagObject + "_TYPE __global " + dataType + "*";
            defines = defines + "\n#define IMAGE_" + tagObject + "_PIXEL_TYPE " + dataType;

            // image size handling
            if (bufferObject->GetDimensions()[2] > 1)
            {
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(bufferObject->GetDimensions()[0]);
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(bufferObject->GetDimensions()[1]);
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH " + std::to_string(bufferObject->GetDimensions()[2]);
            }
            else
            {
                if (bufferObject->GetDimensions()[1] > 1)
                {
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(bufferObject->GetDimensions()[0]);
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT " + std::to_string(bufferObject->GetDimensions()[1]);
                }
                else
                {
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_WIDTH " + std::to_string(bufferObject->GetDimensions()[0]);
                    defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_HEIGHT 1";
                }
                defines = defines + "\n#define IMAGE_SIZE_" + tagObject + "_DEPTH 1";
            }

            // position (dimensionality) handling
            if (bufferObject->GetDimensions()[2] == 1)
            {
                defines = defines + "\n#define POS_" + tagObject + "_TYPE int2";
                if (bufferObject->GetDimensions()[1] == 1) // 1D
                {
                    defines = defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, 0)";
                }
                else // 2D
                {
                    defines = defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int2)(pos0, pos1)";
                }
            }
            else // 3/4D
            {
                defines = defines + "\n#define POS_" + tagObject + "_TYPE int4";
                defines =
                    defines + "\n#define POS_" + tagObject + "_INSTANCE(pos0,pos1,pos2,pos3) (int4)(pos0, pos1, pos2, 0)";
            }

            // read/write images
            std::string sdim = (bufferObject->GetDimensions()[2] == 1) ? "2" : "3";
            defines = defines + "\n#define READ_" + tagObject + "_IMAGE(a,b,c) read_buffer" + sdim + "d" + abbrType +
                    "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
            defines = defines + "\n#define WRITE_" + tagObject + "_IMAGE(a,b,c) write_buffer" + sdim + "d" + abbrType +
                    "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
            defines = defines + "\n";    
        } // end check if not Scalar Object
        else
        {
            std::cout << "\t\t\tthis is NOT a buffer!" << std::endl;
        }

    } // end of for loop on hashmap
    return defines;
}

std::string Kernel::TypeAbbr(const std::string type) const
{
    if (type.compare("float") == 0)
    {
        return "f";
    }
    else if (type.compare("char") == 0)
    {
        return  "c";
    }
    else if (type.compare("uchar") == 0)
    {
        return  "uc";
    }
    else if (type.compare("int") == 0)
    {
        return  "i";
    }
    else if (type.compare("uint") == 0)
    {
        return  "ui";
    }
    return ""; 
}

void Kernel::AddArgumentsToKernel()
{
    cl_int clError;
    if (parameterList.size() != tagList.size())
    {
        std::cerr << "Error: Invalid number of Objects and Parameters for the kernel." << std::endl;
    }
    for(auto it = tagList.begin(); it != tagList.end(); it++ )
    {
        int index = it - tagList.begin();
        if(parameterList.find(it->c_str()) != parameterList.end())
        {

            std::cout << "\t\t" << it->c_str() ;
            std::cout << " is a " << parameterList.at(it->c_str())->GetObjectType() << std::endl;

            if (parameterList.at(it->c_str())->IsObject("cleBuffer"))
            {    
                Buffer* param = dynamic_cast<Buffer*>(parameterList.at(it->c_str()));
                clError = clSetKernelArg(this->GetKernel(), index, sizeof(param->GetData()), &(param->GetData()));
            }
            else if (parameterList.at(it->c_str())->IsObject("cleFloat"))
            {    
                Float* param = dynamic_cast<Float*>(parameterList.at(it->c_str()));
                clError = clSetKernelArg(this->GetKernel(), index, sizeof(param->GetData()), &(param->GetData()));
            }
            else if (parameterList.at(it->c_str())->IsObject("cleInt"))
            {    
                Int* param = dynamic_cast<Int*>(parameterList.at(it->c_str()));
                clError = clSetKernelArg(this->GetKernel(), index, sizeof(param->GetData()), &(param->GetData()));
            }
            if (clError != CL_SUCCESS)
            {
                std::cerr << "Argument error! Fail to set argument : " << getOpenCLErrorString(clError) << std::endl;
                throw clError;
            }
        }
    }
};

void Kernel::AddObject(LightObject* o, std::string t)
{
    if( std::find(tagList.begin(), tagList.end(), t.c_str()) != tagList.end() &&
        parameterList.find(t.c_str()) == parameterList.end() )
    {
        parameterList.insert(std::make_pair(t, o));
    }
    else
    {
        std::cerr << "Error: Invalid kernel parameter tag" << std::endl;
    }
};

void Kernel::CompileKernel()
{
    std::cout << "\tloadSources" << std::endl;  
    // read kernel, defines, and preamble
    std::string kernel_src = LoadSources();

    std::cout << "\tLoadDefines" << std::endl;  
    std::string defines_src = LoadDefines();

    std::cout << "\tLoadPreamble" << std::endl;  
    std::string preambule_src = LoadPreamble();


    // construct final source code
    std::string ocl_src = defines_src + "\n" + preambule_src + "\n" + kernel_src;
    const char *source_str = (ocl_src).c_str();
    size_t source_size = (ocl_src).size();

    std::cout << "\tcreate kernel and program" << std::endl;  

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

    std::cout << "kernel created" << std::endl;  

}

void Kernel::DefineRangeKernel()
{
    cl_int clError;
    size_t global_item_size[3] = {0, 0, 0};
    for (auto itr = parameterList.begin(); itr != parameterList.end(); itr++)
    {

        std::cout << "\t\t" << itr->first << " is a " << itr->second->GetObjectType() << std::endl;

        if (itr->second->GetObjectType().compare("cleBuffer") == 0)
        {    
            Buffer* bufferObject = dynamic_cast<Buffer*>(itr->second);
            for (size_t i = 0; i < 3; i++)
            {
                size_t objectDim = static_cast<size_t>(bufferObject->GetDimensions()[i]);
                global_item_size[i] = std::max(global_item_size[i], objectDim);
            }
        }
    }
    size_t work_dim = 3;
    clError = clEnqueueNDRangeKernel(this->GetCommandQueue(), this->GetKernel(), work_dim, nullptr, global_item_size, nullptr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Execution error! Could not enqueue ND-Range : " << getOpenCLErrorString(clError) << std::endl;
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
