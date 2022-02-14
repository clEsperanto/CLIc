
#ifndef __cleErodeSphereKernel_hpp
#define __cleErodeSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class ErodeSphereKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_erode_sphere.h" 
        };
        
public:
    ErodeSphereKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
};

} // namespace cle

#endif // __cleErodeSphereKernel_hpp
