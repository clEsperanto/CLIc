
#ifndef __cleGreaterConstantKernel_hpp
#define __cleGreaterConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class GreaterConstantKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_greater_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_greater_constant_3d.h" 
        ;

public:
    GreaterConstantKernel(GPU& gpu) : 
        Kernel( gpu,
                "greater_constant",
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

#endif // __cleGreaterConstantKernel_hpp
