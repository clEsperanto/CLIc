

#ifndef __cleExecuteSeparableKernel_h
#define __cleExecuteSeparableKernel_h

#include "cleKernel.h"

namespace cle
{

class ExecuteSeparableKernel : public Kernel
{
private:
    int m_Dim = 1;
    std::array<float, 3> m_Sigma;
    std::array<int, 3> m_KernelSize;

public:
    ExecuteSeparableKernel(GPU& gpu) : 
        Kernel( gpu, 
                "_separable",
                {"dst" , "src", "dim", "N", "s"}
        ){}

    void SetSources(std::map<std::string, std::string>);
    void SetKernelName(std::string);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma(float, float, float);
    void SetKernelSize(int, int, int);
    void Execute();
};

} // namespace cle

#endif // __cleExecuteSeparableKernel_h
