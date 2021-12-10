

#include "cleNotEqualKernel.hpp"

namespace cle
{


NotEqualKernel::NotEqualKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "not_equal",
            {"src0", "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void NotEqualKernel::SetInput1(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void NotEqualKernel::SetInput2(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void NotEqualKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void NotEqualKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
