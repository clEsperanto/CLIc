

#include "cleFlagExistingLabelsKernel.hpp"
#include "cleSetKernel.hpp"

namespace cle
{

FlagExistingLabelsKernel::FlagExistingLabelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "flag_existing_labels",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void FlagExistingLabelsKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void FlagExistingLabelsKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void FlagExistingLabelsKernel::Execute()
{
    auto dst = this->GetParameter<Object>("dst");

    SetKernel set(this->m_gpu);
    set.SetInput(*dst);
    set.SetValue(0);
    set.Execute();

    this->BuildProgramKernel();
    this->SetArguments();
    this->SetGlobalNDRange("src");
    this->EnqueueKernel();
}

} // namespace cle
