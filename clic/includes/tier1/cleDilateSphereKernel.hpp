    
#ifndef __cleDilateSphereKernel_hpp
#define __cleDilateSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DilateSphereKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_dilate_sphere_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_dilate_sphere_3d.h" 
        ;

public:
    DilateSphereKernel(GPU& gpu) : 
        Kernel( gpu,
                "dilate_sphere",
                {"src" , "dst"}
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

#endif // __cleDilateSphereKernel_hpp
