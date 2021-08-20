

#ifndef __cleNonzeroMinimumBoxKernel_hpp
#define __cleNonzeroMinimumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class NonzeroMinimumBoxKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_nonzero_minimum_box_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_nonzero_minimum_box_3d.h" 
        ;

public:
    NonzeroMinimumBoxKernel (std::shared_ptr<GPU> gpu) : 
        Kernel(gpu,
            "nonzero_minimum_box",
            {"dst", "flag_dst", "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetOutputFlag(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleNonzeroMinimumBoxKernel_hpp
