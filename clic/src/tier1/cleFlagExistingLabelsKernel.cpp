

#include "cleFlagExistingLabelsKernel.hpp"
#include "cleSetKernel.hpp"

namespace cle
{

FlagExistingLabelsKernel::FlagExistingLabelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "flag_existing_labels",
            {"dst" , "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}    

void FlagExistingLabelsKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void FlagExistingLabelsKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void FlagExistingLabelsKernel::Execute()
{
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst"));
    SetKernel set(this->m_gpu);
    set.SetInput(*dst);
    set.SetValue(0);
    set.Execute();

    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
