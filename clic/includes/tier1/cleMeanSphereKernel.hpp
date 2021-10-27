
#ifndef __cleMeanSphereKernel_hpp
#define __cleMeanSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MeanSphereKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_mean_sphere_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_mean_sphere_3d.h" 
        ;

public:
    MeanSphereKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadius(int=0, int=0, int=0);
    void Execute();

private:
    int Radius2KernelSize(float) const;
};

} // namespace cle

#endif // __cleMeanSphereKernel_hpp
