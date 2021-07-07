
#ifndef __cleMinimumBoxKernel_h
#define __cleMinimumBoxKernel_h

#include "cleKernel.h"

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
    MinimumBoxKernel(GPU& gpu) : 
        Kernel( gpu,
                "minimum_separable",
                {"dst", "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_2d});
        m_Sources.insert({this->m_KernelName + "_3d", source_3d}); 
    }

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

#endif // __cleMinimumBoxKernel_h
