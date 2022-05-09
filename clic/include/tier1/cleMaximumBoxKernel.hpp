
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
    void SetRadius(int=0, int=0, int=0);
    void Execute();

private:
    int m_x;
    int m_y;
    int m_z;
    int Radius2KernelSize(int) const;
};

} // namespace cle

#endif // __cleMaximumBoxKernel_hpp
