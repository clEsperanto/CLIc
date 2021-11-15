
#ifndef __cleMeanBoxKernel_hpp
#define __cleMeanBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MeanBoxKernel : public Kernel
{
private:
    std::string m_OclHeader2d = 
        #include "cle_mean_separable_2d.h" 
        ;
    std::string m_OclHeader3d = 
        #include "cle_mean_separable_3d.h" 
        ;

public:
    MeanBoxKernel(std::shared_ptr<GPU>);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

private:
    float m_x;
    float m_y;
    float m_z;
    int Radius2KernelSize(float) const;
};

} // namespace cle

#endif // __cleMeanBoxKernel_hpp
