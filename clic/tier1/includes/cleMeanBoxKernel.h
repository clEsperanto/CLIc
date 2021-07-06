
#ifndef __cleMeanBoxKernel_h
#define __cleMeanBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MeanBoxKernel : public Kernel
{
private:
    std::string source_1 = 
        #include "cle_mean_separable_2d.h" 
        ;
    std::string source_2 = 
        #include "cle_mean_separable_3d.h" 
        ;
private:
    float x;
    float y;
    float z;

    int Radius2KernelSize(float);

public:
    MeanBoxKernel(GPU& gpu) : 
        Kernel( gpu, 
                "mean_separable",
                {"dst", "src"}
        )
    {
        m_Sources.insert({this->m_KernelName + "_2d", source_1});
        m_Sources.insert({this->m_KernelName + "_3d", source_2});    
    }

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

};

} // namespace cle

#endif // __cleMeanBoxKernel_h
