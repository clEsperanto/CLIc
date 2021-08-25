
#ifndef __cleMeanBoxKernel_hpp
#define __cleMeanBoxKernel_hpp

#include "cleKernel.hpp"

namespace cle
{
    
class MeanBoxKernel : public Kernel
{
private:
    std::string source_2d = 
        #include "cle_mean_separable_2d.h" 
        ;
    std::string source_3d = 
        #include "cle_mean_separable_3d.h" 
        ;

public:
    MeanBoxKernel (std::shared_ptr<GPU>);

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

#endif // __cleMeanBoxKernel_hpp
