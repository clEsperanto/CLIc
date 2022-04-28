
#include "cleTopHatBoxKernel.hpp"

#include "cleMinimumBoxKernel.hpp"
#include "cleMaximumBoxKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

TopHatBoxKernel::TopHatBoxKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "top_hat_box",
            {"src" , "dst"}
    )
{}    

void TopHatBoxKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void TopHatBoxKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void TopHatBoxKernel::SetRadius(float t_x, float t_y, float t_z)
{
    this->m_x = t_x;
    this->m_y = t_y;
    this->m_z = t_z;
}

void TopHatBoxKernel::Execute()
{
    // get I/O pointers
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");

    auto temp1 = this->m_gpu->Create<float>(src->Shape());
    auto temp2 = this->m_gpu->Create<float>(src->Shape());

    MinimumBoxKernel minimum(this->m_gpu);
    minimum.SetInput(*src);
    minimum.SetOutput(temp1);
    minimum.SetRadius(m_x, m_y, m_z);
    minimum.Execute();

    MaximumBoxKernel maximum(this->m_gpu);
    maximum.SetInput(temp1);
    maximum.SetOutput(temp2);
    maximum.SetRadius(m_x, m_y, m_z);
    maximum.Execute();

    AddImagesWeightedKernel add(this->m_gpu);
    add.SetInput1(*src);
    add.SetInput2(temp2);
    add.SetOutput(*dst);
    add.SetFactor1(1);
    add.SetFactor2(-1);
    add.Execute();
}

} // namespace cle
