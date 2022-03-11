
#ifndef __cleSeparableKernel_hpp
#define __cleSeparableKernel_hpp

#include "cleKernel.hpp"

namespace cle
{

class SeparableKernel : public Kernel
{
public:
    SeparableKernel(std::shared_ptr<GPU>);
    void SetSources(const std::map<std::string, std::string>&);
    void SetKernelName(const std::string&);
    void SetInput(Object&);
    void SetOutput(Object&);
    void SetSize(int);
    void SetSigma(float);
    void SetDimension(int);
    void Execute();
};

} // namespace cle

#endif // __cleSeparableKernel_hpp
