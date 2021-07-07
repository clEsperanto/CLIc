

#ifndef __cleKernel_h
#define __cleKernel_h

#include "clic.h"

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

    // const std::string m_KernelFolder = KERNELS_DIR;  // ocl kernel folder path
    // const std::string m_PreambleFile = PREAMBLE_OCL; // ocl preamble file path

protected:
    GPU m_gpu;

    // kernel specifics
    std::string m_KernelName;
    std::string m_DimensionTag = "";
    std::vector<std::string> m_TagList;
    std::map<std::string, std::string> m_Sources;
    std::map<std::string, std::shared_ptr<cle::LightObject> > m_ParameterList;

    std::array<size_t,3> m_GlobalRange = {{0, 0, 0}};
    size_t m_CurrentHash = 0;
    bool m_BuildProgram = true;

    // Load/Generate OCL Source
    std::string LoadPreamble();
    std::string LoadSources();
    std::string LoadDefines();
    std::string GenerateSources();
    std::string TypeAbbr(const std::string) const;

    // Populate Parameter list with data and tag
    void AddObject(Buffer, std::string);
    void AddObject(int, std::string);
    void AddObject(float, std::string);

    // Build, Set, and Run Kernel
    void ManageDimensions(std::string);
    void BuildProgramKernel();
    void SetArguments();
    void EnqueueKernel();

public:
    Kernel(GPU&, std::string, std::vector<std::string>);
    ~Kernel();

    // virtual abstract method Execute
    // Must be implemented in child kernel class
    virtual void Execute() = 0;
};

} // namespace cle

 

#endif // __cleKernel_h
