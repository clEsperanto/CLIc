
#ifndef __cleSobelKernel_hpp
#define __cleSobelKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class SobelKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_sobel_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_sobel_3d.h" 
        ;

public:
    SobelKernel (std::shared_ptr<GPU> gpu) : 
        Kernel( gpu,
                "sobel",
                {"dst" , "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d});
    }
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
};

} // namespace cle

#endif // __cleSobelKernel_hpp
