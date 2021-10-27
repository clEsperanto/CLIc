    
#ifndef __cleDilateSphereKernel_hpp
#define __cleDilateSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DilateSphereKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_dilate_sphere_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_dilate_sphere_3d.h" 
        ;

public:
    DilateSphereKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
    
};

} // namespace cle

#endif // __cleDilateSphereKernel_hpp
