
#ifndef __cleErodeSphereKernel_hpp
#define __cleErodeSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ErodeSphereKernel : public Kernel
{

private:
    std::string source_2d = 
        #include "cle_erode_sphere_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_erode_sphere_3d.h" 
        ;

public:
    ErodeSphereKernel (std::shared_ptr<GPU>);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void Execute();

};

} // namespace cle

#endif // __cleErodeSphereKernel_hpp
