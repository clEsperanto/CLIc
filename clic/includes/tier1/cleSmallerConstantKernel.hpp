
#ifndef __cleSmallerConstantKernel_hpp
#define __cleSmallerConstantKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SmallerConstantKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_smaller_constant_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_smaller_constant_3d.h" 
        ;

public:
    SmallerConstantKernel (std::shared_ptr<GPU> gpu) : 
        Kernel( gpu,
                "smaller_constant",
                {"src1" , "scalar", "dst"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetConstant(float);
    void Execute();

};

} // namespace cle

#endif // __cleSmallerConstantKernel_hpp
