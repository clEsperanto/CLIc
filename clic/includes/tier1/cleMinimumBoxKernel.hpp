
#ifndef __cleMinimumBoxKernel_hpp
#define __cleMinimumBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MinimumBoxKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_minimum_separable_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_minimum_separable_3d.h" 
        ;

public:
    MinimumBoxKernel (std::shared_ptr<GPU> gpu);

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

private:
    float x;
    float y;
    float z;
    int Radius2KernelSize(float);
};

} // namespace cle

#endif // __cleMinimumBoxKernel_hpp
