

#include "cleCustomKernel.hpp"

#include <fstream>
#include <streambuf>

namespace cle
{

CustomKernel::CustomKernel (std::shared_ptr<GPU> gpu, const char * kernelName, std::vector<std::string> tagList) :    
    Kernel( gpu,kernelName,tagList) {}

void CustomKernel::SetSource(const char * SourceFile)
{
    std::ifstream t(SourceFile);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    m_Sources.insert({m_KernelName, str});
}

void CustomKernel::SetParameter(Buffer& x, const char* tag)
{
    this->AddObject(x, tag);
}

void CustomKernel::SetParameter(float x, const char* tag)
{
    this->AddObject(x, tag);
}

void CustomKernel::SetParameter(int x, const char* tag)
{
    this->AddObject(x, tag);
}

void CustomKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
