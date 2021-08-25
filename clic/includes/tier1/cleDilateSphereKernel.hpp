    
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
    DilateSphereKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();
    
};

} // namespace cle

#endif // __cleDilateSphereKernel_hpp
