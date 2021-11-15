

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

#include "cleLightObject.hpp"
#include "cleObject.hpp"
#include "cleScalar.hpp"
#include "cleBuffer.hpp"
#include "cleImage.hpp"


#include "cleGPU.hpp"


namespace cle
{

class Kernel
{
private:
    cl::Program m_Program;
    cl::Kernel m_Kernel;

    const std::string BufferDefines(std::string&, std::string&, std::string&, int) const;
    const std::string ImageDefines(std::string&, std::string&, std::string&, int) const;

protected:
    std::shared_ptr<GPU> m_gpu;

    // kernel specifics
    std::string m_KernelName ="";
    std::string m_nDimPrefix ="";
    std::vector<std::string> m_Tags;
    std::map<std::string, std::string> m_Sources;
    std::map<std::string, std::shared_ptr<cle::LightObject> > m_Parameters;

    std::array<size_t, 3> m_GlobalRange = {{0, 0, 0}};
    size_t m_CurrentHash = 0;
    bool m_BuildProgram = true;

    // Load/Generate OCL Source
    const std::string LoadPreamble() const;
    const std::string LoadSources() const;
    const std::string LoadDefines() const;
    const std::string GenerateSources() const;
    std::string TypeAbbr(const char*) const;

    // Populate Parameter list with data and tag
    void AddObject(Object&, const char*);
    void AddObject(int, const char*);
    void AddObject(float, const char*);

    // Build, Set, and Run Kernel
    void ManageDimensions(const char*);
    void BuildProgramKernel();
    void SetArguments();
    void EnqueueKernel();

    template<class T=cle::Object>
    std::shared_ptr<T> GetParameter(const char* t_tag) const
    {
        return std::dynamic_pointer_cast<T>(this->m_Parameters.at(t_tag));
    }

public:
    Kernel(std::shared_ptr<GPU>, const char*, const std::vector<std::string>&);
    ~Kernel();

    // virtual abstract method Execute
    // Must be implemented in child kernel class
    virtual void Execute() = 0;
};

} // namespace clic

 

#endif // __cleKernel_h