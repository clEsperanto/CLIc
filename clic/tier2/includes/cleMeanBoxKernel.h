
#ifndef __cleMeanBoxKernel_h
#define __cleMeanBoxKernel_h

#include "cleKernel.h"

namespace cle
{
    
class MeanBoxKernel : public Kernel
{
private:
    float x;
    float y;
    float z;

    int Radius2KernelSize(float);

public:
    MeanBoxKernel(GPU& gpu) : 
        Kernel( gpu, 
                "mean",
                {"dst", "src"}
        )
    {}

    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetRadius(float=0, float=0, float=0);
    void Execute();

};

} // namespace cle

#endif // __cleMeanBoxKernel_h
