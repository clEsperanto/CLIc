
#ifndef __cleMeanBoxKernel_hpp
#define __cleMeanBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MeanBoxKernel : public Kernel
{
private:
    std::string m_OclHeader = {
        #include "cle_mean_separable.h" 
        };

public:
    MeanBoxKernel(std::shared_ptr<GPU>);
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

#endif // __cleMeanBoxKernel_hpp
