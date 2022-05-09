
#ifndef __cleMinimumBoxKernel_hpp
#define __cleMinimumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumBoxKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_minimum_separable.h" 
        };

public:
    MinimumBoxKernel(std::shared_ptr<GPU>);
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

#endif // __cleMinimumBoxKernel_hpp
