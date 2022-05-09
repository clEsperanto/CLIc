
#ifndef __cleMeanSphereKernel_hpp
#define __cleMeanSphereKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MeanSphereKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_mean_sphere.h" 
        };

public:
    MeanSphereKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

private:
    int Radius2KernelSize(float) const;
};

} // namespace cle

#endif // __cleMeanSphereKernel_hpp
