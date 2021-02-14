

#ifndef __cleKernel_h
#define __cleKernel_h

#ifndef CL_HPP_ENABLE_EXCEPTIONS
#   define CL_HPP_ENABLE_EXCEPTIONS
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#   define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#   define CL_HPP_MINIMUM_OPENCL_VERSION 120
#endif

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#   include <CL/cl2.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include "cleLightObject.h"
#include "cleBuffer.h"
#include "cleFloat.h"
#include "cleInt.h"

#include "cleGPU.h"
#include "cleOperations.h"


namespace cle
{

class Kernel
{
private:

    cl::Program m_Program;
    cl::Kernel m_Kernel;
    
    // const std::string m_KernelFolder = "/mnt/data/workspace/C++/clEsperanto/CLIc_v2/clij-opencl-kernels/kernels";
    // const std::string m_PreambleFile =  "/mnt/data/workspace/C++/clEsperanto/CLIc_v2/clic/preamble.cl";

    const std::string m_KernelFolder = KERNELS_DIR; 
    const std::string m_PreambleFile = PREAMBLE_OCL;

protected:
    // std::string m_Preamble;
    // std::map<std::string, std::string> m_Sources;
    std::string m_KernelName;
    std::string m_DimensionTag = "";

    GPU m_gpu;

    std::vector<std::string> m_TagList;
    std::map<std::string, std::shared_ptr<cle::LightObject> > m_ParameterList;

    std::array<size_t,3> m_GlobalRange{{0, 0, 0}};
    size_t m_CurrentHash = 0;
    bool m_BuildProgram = true;

    std::string TypeAbbr(const std::string) const;
    std::string LoadPreamble();
    std::string LoadSources();
    std::string LoadDefines();
    std::string GenerateSources();

    void AddObject(Buffer, std::string);
    void AddObject(int, std::string);
    void AddObject(float, std::string);

    void BuildProgramKernel();
    void SetArguments();
    void EnqueueKernel();
    void ManageDimensions(std::string);

public:
    Kernel(GPU&, std::string, std::vector<std::string>);
    ~Kernel();

    virtual void Execute() = 0;
    
    std::string GetKernelName();
    cl::Kernel GetKernel();
    cl::Program GetProgram();
};

} // namespace cle

 

#endif // __cleKernel_h
