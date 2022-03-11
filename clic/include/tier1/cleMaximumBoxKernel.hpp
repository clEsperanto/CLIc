
#ifndef __cleMaximumBoxKernel_hpp
#define __cleMaximumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MaximumBoxKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_maximum_separable.h" 
        };
        
public:
    MaximumBoxKernel(std::shared_ptr<GPU>);
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

#endif // __cleMaximumBoxKernel_hpp
