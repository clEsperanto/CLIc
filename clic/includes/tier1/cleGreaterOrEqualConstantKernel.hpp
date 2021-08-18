
#ifndef __cleGreaterOrEqualConstantKernel_hpp
#define __cleGreaterOrEqualConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterOrEqualConstantKernel : public Kernel
{


private:
    std::string source_2d = 
        #include "cle_greater_or_equal_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_greater_or_equal_constant_3d.h" 
        ;

public:
    GreaterOrEqualConstantKernel(GPU& gpu) : 
        Kernel( gpu,
                "greater_or_equal_constant",
                {"src1", "scalar", "dst"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetScalar(float);
    void Execute();
};

} // namespace cle

#endif // __cleGreaterOrEqualConstantKernel_hpp
