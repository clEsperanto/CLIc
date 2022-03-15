

#include "cleCustomKernel.hpp"

#include <fstream>
#include <streambuf>

namespace cle
{

CustomKernel::CustomKernel (std::shared_ptr<GPU> t_gpu, const char * t_kernel_name, std::vector<std::string>& t_tags) :    
    Kernel(t_gpu, t_kernel_name, t_tags) {}

void CustomKernel::SetSource(const char* t_source_file)
{
    std::ifstream t(t_source_file);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    this->m_Sources.insert({m_KernelName, str});
}

void CustomKernel::SetParameter(Object& t_x, const char* t_t_tag)
{
    this->AddObject(t_x, t_t_tag);
}

void CustomKernel::SetParameter(float t_x, const char* t_tag)
{
    this->AddObject(t_x, t_tag);
}

void CustomKernel::SetParameter(int t_x, const char* t_tag)
{
    this->AddObject(t_x, t_tag);
}

void CustomKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
