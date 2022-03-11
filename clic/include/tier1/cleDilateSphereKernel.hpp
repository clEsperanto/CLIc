    
#ifndef __cleDilateSphereKernel_hpp
#define __cleDilateSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class DilateSphereKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_dilate_sphere.h" 
        };

public:
    DilateSphereKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void Execute();
    
};

} // namespace cle

#endif // __cleDilateSphereKernel_hpp
