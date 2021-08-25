

#ifndef __cleExecuteSeparableKernel_hpp
#define __cleExecuteSeparableKernel_hpp

#include "cleKernel.hpp"

namespace cle
{

class ExecuteSeparableKernel : public Kernel
{
private:
    int m_Dim = 1;
    std::array<float, 3> m_Sigma;
    std::array<int, 3> m_KernelSize;

public:
    ExecuteSeparableKernel (std::shared_ptr<GPU>);

    void SetSources(std::map<std::string, std::string>);
    void SetKernelName(std::string);
    void SetInput(Buffer&);
    void SetOutput(Buffer&);
    void SetSigma(float, float, float);
    void SetKernelSize(int, int, int);
    void Execute();
};

} // namespace cle

#endif // __cleExecuteSeparableKernel_hpp
