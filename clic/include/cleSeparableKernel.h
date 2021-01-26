
#ifndef __cleSeparableKernel_h
#define __cleSeparableKernel_h

#include "cleKernel.h"

namespace cle
{

class SeparableKernel : public Kernel
{
    
public:
    SeparableKernel(GPU& gpu) : 
        Kernel( gpu, 
                "separable",
                {"dst" , "src", "dim", "N", "s"}
        ){}

    void SetSources(std::map<std::string, std::string>);
    void SetKernelName(std::string);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSize(int);
    void SetSigma(float);
    void SetDimension(int);
    void Execute();
};

} // namespace cle

#endif // __cleSeparableKernel_h
