

#ifndef __cleKernel_h
#define __cleKernel_h

#include "clic.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include "cleDataType.hpp"

#include "cleGPU.hpp"
#include "cleOperations.hpp"


namespace cle
{

class Kernel
{
private:
    cl::Program m_Program;
    cl::Kernel m_Kernel;

protected:
    std::shared_ptr<GPU> m_gpu;

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
    std::string TypeAbbr(const char*) const;

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
    Kernel(std::shared_ptr<GPU>, std::string, std::vector<std::string>);
    ~Kernel();

    // virtual abstract method Execute
    // Must be implemented in child kernel class
    virtual void Execute() = 0;
};

} // namespace clic

 

#endif // __cleKernel_h
