

#include "cleFlagExistingLabelsKernel.h"
#include "cleSetKernel.h"

namespace cle
{

void FlagExistingLabelsKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void FlagExistingLabelsKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void FlagExistingLabelsKernel::Execute()
{
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));
    SetKernel set(this->m_gpu);
    set.SetInput(*dst);
    set.SetValue(0);
    set.Execute();

    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
